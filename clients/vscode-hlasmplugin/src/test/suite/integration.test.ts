/*
 * Copyright (c) 2019 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Broadcom, Inc. - initial API and implementation
 */

import * as assert from 'assert';
import * as vscode from 'vscode';
import * as path from 'path';
import * as helper from './testHelper';

suite('Integration Test Suite', () => {
    const workspace_file = 'open';
    let editor: vscode.TextEditor;

    suiteSetup(async function () {
        this.timeout(30000);

        await helper.showDocument(workspace_file);
        editor = helper.get_editor(workspace_file);
    });

    // open 'open' file, should be recognized as hlasm
    test('HLASM file open test', async () => {
        // setting a language takes a while but shouldn't take longer than a second
        await helper.sleep(1000);
        assert.ok(editor.document.languageId === 'hlasm');
    }).timeout(10000).slow(4000);

    // change 'open' file to create diagnostic
    test('Diagnostic test', async () => {
        // register callback to check for the correctness of the diagnostic
        const diagnostic_event = new Promise<[vscode.Uri, vscode.Diagnostic[]][]>((resolve, reject) => {
            const listener = vscode.languages.onDidChangeDiagnostics((_) => {
                listener.dispose();
                resolve(vscode.languages.getDiagnostics());
            });
        });
        // remove second parameter from LR instruction
        await editor.edit(edit => {
            edit.delete(new vscode.Range(new vscode.Position(2, 6), new vscode.Position(2, 7)));
        });

        const allDiags = await diagnostic_event;
        assert.ok(allDiags.length === 1, 'Wrong number of diagnosed files');
        assert.ok(allDiags[0][0].path === editor.document.uri.path, 'Wrong path');

        const openDiags = allDiags[0][1];
        assert.ok(openDiags.length == 1 && openDiags[0].code == 'M003', 'Wrong diagnostic');
    }).timeout(10000).slow(1000);

    // test completion for instructions
    test('Completion Instructions test', async () => {
        await helper.insertString(editor, new vscode.Position(7, 1), 'L');

        await vscode.commands.executeCommand('editor.action.triggerSuggest');
        await helper.sleep(1000);

        await vscode.commands.executeCommand('acceptSelectedSuggestion');
        await helper.sleep(1000);

        const text = editor.document.getText();
        const acceptedLine = text.split('\n')[7];

        assert.ok(acceptedLine.includes('L   R,D12U(X,B)'), 'Wrong suggestion result' + acceptedLine);
    }).timeout(10000).slow(4000);

    // test completion for variable symbols
    test('Completion Variable symbol test', async () => {
        await helper.insertString(editor, new vscode.Position(8, 0), '&');

        await vscode.commands.executeCommand('editor.action.triggerSuggest');
        await helper.sleep(1000);

        await vscode.commands.executeCommand('acceptSelectedSuggestion')
        await helper.sleep(1000);

        const text = editor.document.getText();
        const acceptedLine = text.split('\n')[8];

        assert.ok(acceptedLine.includes('&VAR'), 'Wrong suggestion result' + acceptedLine);
    }).timeout(10000).slow(4000);

    // go to definition for ordinary symbol
    test('Definition Ordinary symbol test', async () => {
        const result: vscode.Location[] = await vscode.commands.executeCommand('vscode.executeDefinitionProvider', editor.document.uri, new vscode.Position(1, 7));

        assert.ok(result.length == 1
            && result[0].uri.fsPath == editor.document.fileName
            && result[0].range.start.line == 9
            && result[0].range.start.character == 0, 'Wrong ordinary symbol definition location');
    }).timeout(10000).slow(1000);

    // hover for variable symbol
    test('Hover Variable symbol test', async () => {
        const result: vscode.Hover[] = await vscode.commands.executeCommand('vscode.executeHoverProvider', editor.document.uri, new vscode.Position(6, 8));

        assert.ok(result.length == 1
            && result[0].contents.length == 1
            && (result[0].contents[0] as vscode.MarkdownString).value == 'SETA variable', 'Wrong variable symbol hover contents');
    }).timeout(10000).slow(1000);

    // go to definition for macros
    test('Definition Macro test', async () => {
        const result: vscode.Location[] = await vscode.commands.executeCommand('vscode.executeDefinitionProvider', editor.document.uri, new vscode.Position(6, 2));
        assert.ok(result.length == 1
            && result[0].uri.fsPath == path.join(helper.getWorkspacePath(), 'libs', 'mac.asm')
            && result[0].range.start.line == 1
            && result[0].range.start.character == 4, 'Wrong macro definition location');
    }).timeout(10000).slow(1000);

    // debug open code test
    test('Debug test', async () => {
        const session = await helper.debugStartSession();

        // step over once
        await helper.debugStepOver(1);
        // then check for VAR2 variable
        const scopesResult = await session.customRequest('scopes', { frameId: 0 });

        const scopes = scopesResult.body ? scopesResult.body.scopes : scopesResult.scopes;

        const reference = scopes.find((scope: { name: string }) => scope.name == 'Locals').variablesReference;
        const variablesResult = await session.customRequest('variables', { variablesReference: reference });

        const variables = variablesResult.body ? variablesResult.body.variables : variablesResult.variables;

        assert.ok(variables.length == 1 && variables[0].value == 'SOMETHING' && variables[0].name == '&VAR2', 'Wrong debug variable &VAR2');

        // Continue by stepping into a macro and checking the file has been accessed
        await helper.debugStepOver(3);
        await helper.debugStepInto();

        assert.ok(vscode.window.activeTextEditor.document.uri.fsPath === path.join(helper.getWorkspacePath(), 'libs', 'mac.asm'), 'Wrong macro file entered');

        await helper.debugStepOver(3);
        assert.ok(vscode.window.activeTextEditor.document.uri.fsPath === editor.document.uri.fsPath, 'Stepped out to a wrong file');

        await helper.debugStop();

    }).timeout(20000).slow(10000);

    test('Breakpoint test', async () => {
        await helper.toggleBreakpoints(path.join('libs', 'mac.asm'), [3]);
        await helper.toggleBreakpoints('open', [3, 9]);

        await helper.debugStartSession();

        // Continue until breakpoint is hit
        await helper.debugContinue();
        assert.ok(vscode.window.activeTextEditor.document.uri.fsPath === editor.document.uri.fsPath, 'Expected to be in the source file');

        // Continue until breakpoint is hit
        await helper.debugContinue();
        assert.ok(vscode.window.activeTextEditor.document.uri.fsPath == path.join(helper.getWorkspacePath(), 'libs', 'mac.asm'), 'Expected to be in the macro file');

        // Continue until breakpoint is hit
        await helper.debugContinue();
        assert.ok(vscode.window.activeTextEditor.document.uri.fsPath === editor.document.uri.fsPath, 'Expected to be in the source file');

        await helper.debugStop();

    }).timeout(20000).slow(10000);

    // verify that virtual files are working
    test('Virtual files', async () => {
        await helper.toggleBreakpoints('virtual', [7, 11, 12]);

        await helper.debugStartSession();

        // Continue until breakpoint is hit
        await helper.debugContinue();
        assert.ok(vscode.window.activeTextEditor.document.uri.fsPath === path.join(helper.getWorkspacePath(), 'virtual'), 'Expected to be in the source file');

        // Step into a virtual file
        await helper.debugStepInto();
        assert.ok(vscode.window.activeTextEditor.document.uri.path === '/AINSERT:1.hlasm', 'Expected to be in the virtual AINSERT file');

        // Continue until breakpoint is hit and enter the virtual file again
        await helper.debugContinue();
        await helper.debugStepInto();
        assert.ok(vscode.window.activeTextEditor.document.uri.path === '/AINSERT:1.hlasm', 'Expected to be in the virtual AINSERT file');

        // Continue until breakpoint is hit and enter virtual file through generated macro
        await helper.debugContinue();
        assert.ok(vscode.window.activeTextEditor.document.uri.fsPath === path.join(helper.getWorkspacePath(), 'virtual'), 'Expected to be in the source file');

        await helper.debugStop();
    }).timeout(20000).slow(10000);

    // verify that library patterns are working
    test('Test library patterns', async () => {
        const files = await vscode.workspace.findFiles('pattern_test/test_pattern.hlasm');

        assert.ok(files && files[0]);
        const file = files[0];

        // open the file
        const document = await vscode.workspace.openTextDocument(file);

        await vscode.window.showTextDocument(document);

        await helper.sleep(2000);

        const allDiags = vscode.languages.getDiagnostics();
        const patternDiags = allDiags.find(pair => pair[0].path.endsWith("test_pattern.hlasm"))

        if (patternDiags)
            assert.ok(patternDiags[1].length == 0, 'Library patterns are not working');
    }).timeout(10000).slow(2500);
});
