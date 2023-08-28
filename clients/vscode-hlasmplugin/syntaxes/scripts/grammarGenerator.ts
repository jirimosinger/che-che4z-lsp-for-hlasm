var fs = require('fs')

const grammar_details_template = './syntaxes/scripts/hlasm_grammar_template.txt';
const grammar_base_template = './syntaxes/scripts/hlasm_base_template.txt';

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

interface GrammarFile {
  file: string;
  grammarName: string;
  scope: string;
}

interface GrammarDetails extends GrammarFile {
  entryPattern: string;
  codeBlockBegin: string;
  beginLineSkipRule: string;
  listingOffset: string;
}

const hlasmGeneralGrammar: GrammarDetails = {
  file: './syntaxes/hlasmGeneral.tmLanguage.json',
  grammarName: 'IBM HLASM General',
  scope: 'hlasmGeneral',
  entryPattern: 'hlasm_syntax',
  codeBlockBegin: '',
  beginLineSkipRule: '',
  listingOffset: ''
}

const hlasmListingGeneralGrammar: GrammarDetails = {
  file: './syntaxes/hlasmListingGeneral.tmLanguage.json',
  grammarName: 'IBM HLASM Listing General',
  scope: 'hlasmListingGeneral',
  entryPattern: 'code_block',
  codeBlockBegin: code_block_listing_begin,
  beginLineSkipRule: code_block_listing_annotation_length,
  listingOffset: ''
}

const hlasmListingGeneralLongGrammar: GrammarDetails = {
  file: './syntaxes/hlasmListingGeneralLong.tmLanguage.json',
  grammarName: 'IBM HLASM Listing General Long',
  scope: 'hlasmListingGeneralLong',
  entryPattern: 'code_block',
  codeBlockBegin: code_block_listing_long_begin,
  beginLineSkipRule: code_block_listing_long_annotation_length,
  listingOffset: ''
}

const hlasmListingEndevorGrammar: GrammarDetails = {
  file: './syntaxes/hlasmListingEndevor.tmLanguage.json',
  grammarName: 'IBM HLASM Listing Endevor',
  scope: 'hlasmListingEndevor',
  entryPattern: 'code_block',
  codeBlockBegin: code_block_listing_begin,
  beginLineSkipRule: code_block_listing_annotation_length,
  listingOffset: '.'
}

const hlasmListingEndevorLongGrammar: GrammarDetails = {
  file: './syntaxes/hlasmListingEndevorLong.tmLanguage.json',
  grammarName: 'IBM HLASM Listing Endevor Long',
  scope: 'hlasmListingEndevorLong',
  entryPattern: 'code_block',
  codeBlockBegin: code_block_listing_long_begin,
  beginLineSkipRule: code_block_listing_long_annotation_length,
  listingOffset: '.'
}

interface GrammarBase extends GrammarFile {
  includedGrammars: GrammarDetails[];
}

const hlasmBase: GrammarBase = {
  file: './syntaxes/hlasm.tmLanguage.json',
  grammarName: 'IBM HLASM',
  scope: 'hlasm',
  includedGrammars: [hlasmGeneralGrammar]
}

const hlasmListingBase: GrammarBase = {
  file: './syntaxes/hlasmListing.tmLanguage.json',
  grammarName: 'IBM HLASM Listing',
  scope: 'hlasmListing',
  includedGrammars: [
    hlasmListingGeneralGrammar,
    hlasmListingGeneralLongGrammar,
    hlasmListingEndevorGrammar,
    hlasmListingEndevorLongGrammar
  ]
}

function generateGrammarsDetails(props: GrammarDetails) {
  const listingDetails = props.listingOffset + props.beginLineSkipRule;

  fs.readFile(grammar_details_template, 'utf8', (err: Error, data: string) => {
    if (err) {
      return console.log(err);
    }

    let result = data.replaceAll('${grammarName}$', props.grammarName);
    result = result.replaceAll('${scope}$', props.scope);
    result = result.replaceAll('${entryPattern}$', props.entryPattern);
    result = result.replaceAll('${codeBlockBegin}$', props.codeBlockBegin);
    result = result.replaceAll('${listingOffset}$', props.listingOffset);
    result = result.replaceAll('${listingDetails}$', listingDetails);

    fs.writeFile(props.file, result, 'utf8', (err: any) => {
      if (err) return console.log(err);
    });
  });
}

function generateGrammarBase(props: GrammarBase) {
  fs.readFile(grammar_base_template, 'utf8', (err: Error, data: string) => {
    let includeRule = 'include: #{}';

    if (err) {
      return console.log(err);
    }

    if (props.includedGrammars.length !== 0) {
      includeRule = '{"include": "source.' + props.includedGrammars[0].scope + '"}'

      for (let i = 1; i < props.includedGrammars.length; ++i) 
        includeRule += ',{"include": "source.' + props.includedGrammars[i].scope + '"}'
    }

    let result = data.replaceAll('${grammarName}$', props.grammarName);
    result = result.replaceAll('${scope}$', props.scope);
    result = result.replaceAll('${patterns}$', includeRule);

    fs.writeFile(props.file, result, 'utf8', (err: any) => {
      if (err) return console.log(err);
    });
  });
}

generateGrammarsDetails(hlasmGeneralGrammar);
generateGrammarsDetails(hlasmListingGeneralGrammar);
generateGrammarsDetails(hlasmListingGeneralLongGrammar);
generateGrammarsDetails(hlasmListingEndevorGrammar);
generateGrammarsDetails(hlasmListingEndevorLongGrammar);

generateGrammarBase(hlasmBase);
generateGrammarBase(hlasmListingBase);