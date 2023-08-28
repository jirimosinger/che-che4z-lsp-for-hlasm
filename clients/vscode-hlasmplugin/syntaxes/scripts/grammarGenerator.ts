var fs = require('fs')

const grammar_template = './syntaxes/scripts/hlasm_grammar_template.txt';

const code_block_listing_begin = '.{2}Loc  Object Code    Addr1 Addr2  Stmt   Source Statement.*';
const code_block_listing_long_begin = '.{2}Loc    Object Code      Addr1    Addr2    Stmt  Source Statement.*';
const code_block_listing_annotation_length = '.{40}';
const code_block_listing_long_annotation_length = '.{49}';

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
  beginLineSkipRule: string;
  listingOffset: string;
}

const hlasmGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasm.tmLanguage.json',
  grammarName: 'IBM HLASM',
  scope: 'hlasm',
  entryPattern: 'hlasm_syntax',
  codeBlockBegin: '',
  beginLineSkipRule: '',
  listingOffset: ''
}

const hlasmListingGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasmListing.tmLanguage.json',
  grammarName: 'IBM HLASM Listing',
  scope: 'hlasmListing',
  entryPattern: 'code_block',
  codeBlockBegin: code_block_listing_begin,
  beginLineSkipRule: code_block_listing_annotation_length,
  listingOffset: ''
}

const hlasmListingLongGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasmListingLong.tmLanguage.json',
  grammarName: 'IBM HLASM Listing Long',
  scope: 'hlasmListingLong',
  entryPattern: 'code_block',
  codeBlockBegin: code_block_listing_long_begin,
  beginLineSkipRule: code_block_listing_long_annotation_length,
  listingOffset: ''
}

const hlasmListingEndevorGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasmListingEndevor.tmLanguage.json',
  grammarName: 'IBM HLASM Listing Endevor',
  scope: 'hlasmListingEndevor',
  entryPattern: 'code_block',
  codeBlockBegin: code_block_listing_begin,
  beginLineSkipRule: code_block_listing_annotation_length,
  listingOffset: '.'
}

const hlasmListingEndevorLongGrammar: GrammarProperties = {
  grammarDestination: './syntaxes/hlasmListingEndevorLong.tmLanguage.json',
  grammarName: 'IBM HLASM Listing Endevor Long',
  scope: 'hlasmListingEndevorLong',
  entryPattern: 'code_block',
  codeBlockBegin: code_block_listing_long_begin,
  beginLineSkipRule: code_block_listing_long_annotation_length,
  listingOffset: '.'
}

function generate(props: GrammarProperties) {
  const listingDetails = props.listingOffset + props.beginLineSkipRule;

  fs.readFile(grammar_template, 'utf8', (err: Error, data: string) => {
    if (err) {
      return console.log(err);
    }

    let result = data.replaceAll('${grammarName}$', props.grammarName);
    result = result.replaceAll('${scope}$', props.scope);
    result = result.replaceAll('${entryPattern}$', props.entryPattern);
    result = result.replaceAll('${codeBlockBegin}$', props.codeBlockBegin);
    result = result.replaceAll('${listingOffset}$', props.listingOffset);
    result = result.replaceAll('${listingDetails}$', listingDetails);

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
