var fs = require('fs')

const grammar_template = './syntaxes/scripts/hlasm_grammar_template.txt';

enum GrammarTypes {
  Main = 1,
  ListingLong,
  Listing,
  ListingEndevorLong,
  ListingEndevor
}

interface GrammarProperties {
  grammarDestination: string;
  grammarName: string;
  scope: string;
  entryPattern: string;
  codeBlockBegin: string;
  codeBlockEnd: string;
  beginLineSkipRule: string;
  endLineRule: string;
  hlasmLineContent: string;
  beginAsmaRule: string;
}

const hlasmGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasm.tmLanguage.json',
  grammarName: 'IBM HLASM',
  scope: 'hlasm',
  entryPattern: 'hlasm_syntax',
  codeBlockBegin: '',
  codeBlockEnd: '',
  beginLineSkipRule: '^',
  endLineRule: '((?<=^.{71}\\\\s.*)|(?<=^.{0,71}))$',
  hlasmLineContent: '^.{0,71}',
  beginAsmaRule: '^\\\\*\\\\*'
}

const hlasmListingGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasmListing.tmLanguage.json',
  grammarName: 'IBM HLASM Listing',
  scope: 'hlasmListing',
  entryPattern: 'code_block',
  codeBlockBegin: '^.{2}Loc  Object Code    Addr1 Addr2  Stmt   Source Statement.*',
  codeBlockEnd: '^                                                 Relocation Dictionary|                                          Macro and Copy Code Source Summary',
  beginLineSkipRule: '^.{40}',
  endLineRule: '((?<=^.{40}.{71}\\\\s.*)|(?<=^.{40}.{0,71}))$',
  hlasmLineContent: '^.{40}.{0,71}',
  beginAsmaRule: '^\\\\*\\\\*'
}

const hlasmListingLongGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasmListingLong.tmLanguage.json',
  grammarName: 'IBM HLASM Listing Long',
  scope: 'hlasmListingLong',
  entryPattern: 'code_block',
  codeBlockBegin: '^.{2}Loc    Object Code      Addr1    Addr2    Stmt  Source Statement.*',
  codeBlockEnd: '^                                                 Relocation Dictionary',
  beginLineSkipRule: '^.{49}',
  endLineRule: '((?<=^.{49}.{71}\\\\s.*)|(?<=^.{49}.{0,71}))$',
  hlasmLineContent: '^.{49}.{0,71}',
  beginAsmaRule: '^\\\\*\\\\*'
}

const hlasmListingEndevorGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasmListingEndevor.tmLanguage.json',
  grammarName: 'IBM HLASM Listing Endevor',
  scope: 'hlasmListingEndevor',
  entryPattern: 'code_block',
  codeBlockBegin: '^.{3}Loc  Object Code    Addr1 Addr2  Stmt   Source Statement.*',
  codeBlockEnd: '^(1.{8})                                         Relocation Dictionary',
  beginLineSkipRule: '^.{41}',
  endLineRule: '((?<=^.{41}.{71}\\\\s.*)|(?<=^.{41}.{0,71}))$',
  hlasmLineContent: '^.{41}.{0,71}',
  beginAsmaRule: '^.\\\\*\\\\*'
}

const hlasmListingEndevorLongGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasmListingEndevorLong.tmLanguage.json',
  grammarName: 'IBM HLASM Listing Endevor Long',
  scope: 'hlasmListingEndevorLong',
  entryPattern: 'code_block',
  codeBlockBegin: '^.{3}Loc    Object Code      Addr1    Addr2    Stmt  Source Statement.*',
  codeBlockEnd: '^(1.{8})                                         Relocation Dictionary',
  beginLineSkipRule: '^.{50}',
  endLineRule: '((?<=^.{50}.{71}\\\\s.*)|(?<=^.{50}.{0,71}))$',
  hlasmLineContent: '^.{50}.{0,71}',
  beginAsmaRule: '^.\\\\*\\\\*'
}

function generate(props: GrammarProperties) {
  fs.readFile(grammar_template, 'utf8', (err: Error, data: string) => {
    if (err) {
      return console.log(err);
    }

    let result = data.replaceAll('${grammarName}$', props.grammarName);
    result = result.replaceAll('${scope}$', props.scope);
    result = result.replaceAll('${entryPattern}$', props.entryPattern);
    result = result.replaceAll('${codeBlockBegin}$', props.codeBlockBegin);
    result = result.replaceAll('${codeBlockEnd}$', props.codeBlockEnd);
    result = result.replaceAll('${endLineRule}$', props.endLineRule);
    result = result.replaceAll('${beginLineSkipRule}$', props.beginLineSkipRule);
    result = result.replaceAll('${hlasmLineContent}$', props.hlasmLineContent);
    result = result.replaceAll('${beginAsmaRule}$', props.beginAsmaRule);

    fs.writeFile(props.grammarDestination, result, 'utf8', (err: any) => {
      if (err) return console.log(err);
    });
  });
}

generate(hlasmGrammar);
generate(hlasmListingGrammar);
generate(hlasmListingLongGrammar);
generate(hlasmListingEndevorGrammar);
generate(hlasmListingEndevorLongGrammar);
