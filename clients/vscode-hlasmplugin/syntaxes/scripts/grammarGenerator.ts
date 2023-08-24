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
  headerSkipRule: string;
  beginLineSkipRule: string;
  endLineRule: string;
  hlasmLineContent: string;
}

const hlasmGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasm.tmLanguage.json',
  grammarName: 'IBM HLASM',
  scope: 'hlasm',
  headerSkipRule: '',
  beginLineSkipRule: '^',
  endLineRule: '((?<=^.{71}\\\\s.*)|(?<=^.{0,71}))$',
  hlasmLineContent: '^.{0,71}'
}

const hlasmListingEndevorGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasmListingEndevor.tmLanguage.json',
  grammarName: 'IBM HLASM Listing Endevor',
  scope: 'hlasmListingEndevor',
  headerSkipRule: '',
  beginLineSkipRule: '^',
  endLineRule: '((?<=^.{71}\\\\s.*)|(?<=^.{0,71}))$',
  hlasmLineContent: '^.{0,71}'
}


function generate(props: GrammarProperties) {
  fs.readFile(grammar_template, 'utf8', (err: Error, data: string) => {
    if (err) {
      return console.log(err);
    }

    let result = data.replaceAll('${grammarName}$', props.grammarName);
    result = result.replaceAll('${scope}$', props.scope);
    result = result.replaceAll('${endLineRule}$', props.endLineRule);
    result = result.replaceAll('${beginLineSkipRule}$', props.beginLineSkipRule);
    result = result.replaceAll('${hlasmLineContent}$', props.hlasmLineContent);

    fs.writeFile(props.grammarDestination, result, 'utf8', (err: any) => {
      if (err) return console.log(err);
    });
  });
}

generate(hlasmGrammar);
