## [1.3.0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/compare/1.2.0...1.3.0) (2022-06-30)


### Features

* CICS preprocessor now recognizes DFHVALUE constants ([a13e10d](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/a13e10dd9525f383fca222bff6693749df492ab0))
* Conditional DB2 preprocessor ([2b8e38b](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/2b8e38b185e426335635f2caa586d3dd20006b7e))
* CXD instruction support ([19466b4](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/19466b40f680a1b90085dab7f2890285da987847))
* Enhanced commands for continuation handling and trimming of oversized lines ([675256e](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/675256e3161876c97f0dd7b49c0ec4726f18ce11))
* Home directory substitution is now supported in proc_grps.json and pgm_conf.json ([83271c5](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/83271c50ff877104eec32430147dc265977112f8)), closes [eclipse/che-che4z-lsp-for-hlasm#221](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/221)
* Machine option + JSON synonyms validation ([ea71391](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/ea71391dfd37c53ad5658fd19119ecf78613e47b))
* Provide the name of a missing variable or an ordinary symbol in diagnostics ([9ba5294](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/9ba5294f5835b1371dd9595c21ffb22f7b2c49ea)), closes [eclipse/che-che4z-lsp-for-hlasm#265](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/265)
* Show hexadecimal offsets and lengths in hover texts ([1a5e8a3](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/1a5e8a334e3430539a58e2c3ba583eccfafe1356))
* Support for SYSVER system variable ([1af16d0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/1af16d0c60eaa874172854390f2684cd06c79003))
* SYSIN_DSN and SYSIN_MEMBER support ([dde525d](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/dde525db8e6b6c5f5b40868a346b840ed9fdfc17))
* Visual Studio Code workspace variables can be used in pgm_conf.json and proc_grps.json ([e396b17](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/e396b174ca1d29526b38fa5f18a763a439f35b08))


### Fixes

* BASSM has the RR instruction format ([5304035](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/530403574434b542766d1f2bd5a6c6926ace6ff6))
* Changes in configuration files were not detected on Linux ([ebc1ae8](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/ebc1ae8b1a9462e8f9856b99f3b1966a256a39b4))
* DB2 preprocessor incorrectly processes line continuations from included members ([0acf426](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/0acf426e2e3b524801bccf53b8ee452f803d3200))
* Incorrect attribute parsing in CA expressions without spaces between individual terms ([821fae0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/821fae08ee83f0c301c2685afacf3320ce9f62fe)), closes [eclipse/che-che4z-lsp-for-hlasm#263](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/263)
* Incorrect attribute values generated when literals are substituted in CA expressions ([861c09c](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/861c09cf890b79f5292ad7069608cde7fa9d35b8))
* Incorrect remark parsing in CA statements ([4e4a516](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/4e4a516feafb41a954c5c0e70cb6ebb2863cbb80))
* Language server may crash while processing an unexpected operand ([dd714da](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/dd714da2d4b30a8a054addf8ccb0c45efec688aa))
* Parsing of numeric nominal values must be case insensitive ([3b05e1c](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/3b05e1c2536f2979697dd6bc21f43593ec9483df))
* Partially resolved value used as the final value by EQU statement ([0899cf9](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/0899cf9e40a144a9f13c11bbd5e6848fa1207acf))
* References to CA variables in strings are not reported ([3881b55](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/3881b552bc4dd624e8991688dff662c4abe1a8e8))
* Remove deprecated property from the default configuration file ([790e128](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/790e128625350a7dd1e74a5f7227047e4ccec3aa))
* Return correct variable type for values provided in the macro's name field ([39f7ce2](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/39f7ce2e10504ffb573ca611604591a423a0d192))
* Revise machine instructions ([be53133](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/be53133ad15b4e276ab810a982bc335e679e1896))
* String functions are not recognized in concatenations ([79456d5](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/79456d53e79e4aa9d2a9339489bee130aa2a2cac))
* Structured macro variables were not forwarded correctly when a dot separator was used in the macro operand ([dcb5aeb](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/dcb5aebf87d4c5a6928c702f46ed912b83cb460b))
* The language server may crash after reloading the configuration ([860d453](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/860d4532a934306640e0e4495c2ae7e6222f1a56)), closes [eclipse/che-che4z-lsp-for-hlasm#271](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/271)
* The language server may crash when a complex expression is used as a variable symbol index ([e49745f](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/e49745f26621165f9a233b1b0ab6a7108078164e))


### Other changes

* AINSERT grammar tests improvements ([e2fa7d8](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/e2fa7d88ca908208dbefe62bf91fa8583c8159a8))
* Preprocessors ([7ea7ce8](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/7ea7ce80dfa2ff3eecf6c898fee478c0b81fdf68))
* Reduce header dependencies ([2a7f8cb](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/2a7f8cb640884201e3911979961252e7104c10fd))
* Replace internal path uses by URIs ([2c1aab6](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/2c1aab6d3a14fb26b2f7138ed4af55fd43e06bb4))
* Replace internal path uses by URIs [#2](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/2) ([c2d5593](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/c2d55930e432f655aea0fdb0d2648752859e51d2))
* URI representation ([1f664c2](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/1f664c29550f8b8551af317073f891d04953c932))

## [1.2.0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/compare/1.1.0...1.2.0) (2022-05-11)


### Features

* Allow viewing content generated by AINSERT instruction and preprocessors ([#248](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/248)) ([5313215](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/5313215ebfd120856c49c410212da4f9919f93d0))
* Assembler options can be specified in pgm_conf.json to override values from proc_grps.json ([bf24968](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/bf24968e1d62b5c888ec82e3b72fba55421bb0f4))
* Basic GOFF, XOBJECT and SYSOPT_XOBJECT support ([3e6daed](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/3e6daedc53fcda956bf59ae5e9a3a36d82db643e))
* DB2 preprocessor VERSION option ([#255](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/255)) ([2c032cc](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/2c032cc20bf82f0ea281063b5d66ca459da9ee0f)), closes [#229](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/229)
* Expand the list of associated file extensions ([#256](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/256)) ([3864bd9](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/3864bd9b93164a62be126a0b1eb5ef17cb583ec7))
* Instruction set versioning support ([f9532c5](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/f9532c54b194261b9e67d46760d91ab93bbca579))
* MNOTE support ([7ac224c](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/7ac224cbec1337defcf85f36e75299f1003fe408))


### Other changes

* Added missing tests from Rally ([6b59233](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/6b59233475e2112ac61fb4ee569916428b5b5d18))


### Fixes

* Double lock while closing files ([#254](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/254)) ([992405e](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/992405e8c8f07082d65da1a916cf0ca691918d45))
* Empty arrays now behave similarly to other subscripted variables in the macro tracer ([ce0ba21](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/ce0ba21b97de4965b90cdd3dc8ff098ea6007c20))
* Empty operands ignored by the SETx instructions ([5d13a69](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/5d13a6987a1d189a56874d3016732d3b35b1c540))
* Forward structured macro parameters correctly ([89d2936](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/89d2936c35534474ad17075794e5de3fea289796))
* Improve detection of HLASM files ([21521e4](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/21521e49f1aee96b1d5e1187e3f96c9ceb81cd03))
* Incomplete conditional assembly expression evaluation ([08c5429](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/08c54290bd3520e9f7a8f145b020840b90aef623))
* Incorrect operator precedence in conditional assembly expressions ([0d9764e](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/0d9764e9e01d65b34f9513956f52494077f28c25)), closes [eclipse/che-che4z-lsp-for-hlasm#259](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/259)
* Lifetime issues in the file manager ([#257](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/257)) ([c3d5d90](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/c3d5d906c29c3246056aa5669d00815818555cfa))
* N' attribute and subscript array contents ([#249](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/249)) ([31cc66f](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/31cc66f60bc3a1503bd55730c877d964a325f574)), closes [#231](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/231)
* Parsing of negative numbers in machine expressions ([fee9e33](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/fee9e334aee7050e551a8c8c1f82a6a3df930498))
* Reaching internal ACTR limit now only generates warnings ([a223b48](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/a223b48e1cd19df117e4b18c297036b51572b4e7))

## [1.1.0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/compare/1.0.0...1.1.0) (2022-03-29)


### Features

* Added support for SYSSTMT ([#234](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/234)) ([2d3da68](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/2d3da6819ae9aa1167120815e3168cfbf76773f0)), closes [#213](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/213)
* Instruction operand checking utilizes USING map ([#242](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/242)) ([5e77dac](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/5e77dacdbfa3c171b4f7a9e964703ccca3fec168))
* USING and DROP support ([#230](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/230)) ([f9dfd05](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/f9dfd05bee2d344241ea92ec0ba4879dd93e3710))


### Other changes

* Expressions should not collect diagnostics ([#235](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/235)) ([45c2711](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/45c271103e1740710abf014ff2e7338a7a2bd2e3))
* Reduce data definition length evaluation dependencies ([#239](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/239)) ([b07419f](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/b07419ff31e2d8c8b05acb28e440cddeb35e1646))
* Use string_view in instructions and diagnostics ([#226](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/226)) ([0652b66](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/0652b66cc5b366fec3a1f787f60f5ab26a869600))


### Fixes

* AINSERT immediate variable evaluation ([#243](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/243)) ([e8006e9](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/e8006e9b4c8e1ba262781e6e2a10e8296913b254)), closes [#236](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/236)
* Cross-section relative immediate references should be only warned upon ([#240](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/240)) ([fe243de](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/fe243def88410643af5d4060b5959eafab527dc7))
* Document navigation does not work correctly in Untitled documents ([#247](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/247)) ([4316546](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/4316546c5a56c1c1de729b07444042a8a65de4ba))
* Incorrect module layout computed when ORG instruction is used in sections with multiple location counters ([#241](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/241)) ([e5838c5](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/e5838c5e026d85a178111f95895336e7202dd302))
* Single char strings highlighting ([#246](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/246)) ([37cd2d7](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/37cd2d71cc936d07a93fc0e663d74a1c47c65ccd))
* System variables with subscripts ([#233](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/233)) ([f01e81b](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/f01e81b7656661104c9f68e0e6819fc116e01c64)), closes [#212](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/212)

## [1.0.0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/compare/0.15.1...1.0.0) (2022-01-31)


### ⚠ BREAKING CHANGES

* Release V1

### Features

* Literal support ([#207](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/207)) ([44a93f2](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/44a93f28a124ba873811f0a5f314e02c1693655f)), closes [#193](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/193) [#75](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/75)
* Location counter length attribute support ([#208](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/208)) ([c2d64c9](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/c2d64c9023ce438af7e3737ced4330a9f5f805bb)), closes [#71](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/71)
* Toleration of EXEC CICS statements and related preprocessing ([#219](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/219)) ([5e10293](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/5e10293594d96dcd64446de49c68a83e98a13ca9))


### Fixes

* Data definition grammar is too greedy ([#223](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/223)) ([dd9b557](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/dd9b55773cb887c6e538872529fbcf5e408f6dca))
* Improve statement parsing performance ([#201](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/201)) ([c949784](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/c949784a87fb4b48dfdd6c37e54c4e205711da9d))
* Incorrect module layout generated when data definition operands have different alignments ([#210](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/210)) ([8660232](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/8660232500d8ce38ea1f8d5fe02bac15c590fe2c))
* Provide semantic tokens through language client 7.0.0 ([#211](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/211)) ([42a830b](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/42a830b52586b358f9ef73dbe72a41a6ce8d7357)), closes [#82](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/82)


* Release V1 ([312295a](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/312295a53f9b30245dce3f4c07bf05ba4246625e))


### Other changes

* Readme update ([35800e5](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/35800e57e560323dd21d24d1a1fe171ea56f8de8))

### [0.15.1](https://github.com/eclipse/che-che4z-lsp-for-hlasm/compare/0.15.0...0.15.1) (2021-11-11)


### Fixes

* Improve outline performance for files with large number of labels ([#202](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/202)) ([16def4b](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/16def4bb701049a031d7b66fb876daa382c3d4bd))
* Language server must tolerate extra arguments passed by vscode in wasm mode ([#203](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/203)) ([f7f4fa6](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/f7f4fa6bc6e3555ed093ebf1fcc7f07ff1fad37c))


### Other changes

* Add changelog for v0.15.1 ([d86d752](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/d86d752f36512604ecf930b561e402cc8dc77afb))

## [0.15.0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/compare/0.14.0...0.15.0) (2021-11-08)


### Features

* Allow wildcards in proc_grps.json library specification ([#172](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/172)) ([9157ef7](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/9157ef78e903c80e484eaa6ec6f6970bd4647093)), closes [#69](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/69)
* Document outline support ([9f65cd4](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/9f65cd4f6a473a349639b49b9836b020ad507788))
* Implement SYSTEM_ID system variable ([#182](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/182)) ([caa6fd0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/caa6fd04b380a6735a1379ff77f04b25c99b4cc2))
* Support START and MHELP instructions ([#171](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/171)) ([f9f2fb2](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/f9f2fb2852a7feed5e1fdca4a771588b15758762))
* Telemetry reporting ([#187](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/187)) ([70445dd](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/70445ddd3db3d1a8344aa151c7651c5e802459aa))


### Other changes

* Add Support section to client readme ([#192](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/192)) ([c68270f](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/c68270f53cfd9f6bf15289d35d75bb8fa75ddb84))
* replace theia docker images that do not exist anymore ([#197](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/197)) ([ac5cb3d](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/ac5cb3df74f0a80d8cab481469255e251854f3f8))


### Fixes

* &SYSMAC should contain only the macro name (fixes eclipse/che-che4z-lsp-for-hlasm[#168](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/168)) ([4e2fddc](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/4e2fddce850712783d6cfb14091af21692076a53))
* AINSERT operand length validation ([#196](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/196)) ([03d62ad](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/03d62ad65d1ed4167f60f7e41c6319118955dca5))
* Apostrophe parsing in model statements (fixes eclipse/che-che4z-lsp-for-hlasm[#163](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/163)) ([#164](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/164)) ([94843c8](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/94843c8a8facdc621058b00100129331da67e66a))
* Diagnostics lost during JSON serialization ([#170](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/170)) ([f272f7d](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/f272f7d9820488a9ecda52d909679f67507cf2b7))
* DOT operator in string concatenation is optional ([#190](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/190)) ([33d9ecf](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/33d9ecfd14b7a9418418c640d164f7c1f6c73fc0))
* Enhance conditional assembler expression parsing, evaluation and diagnostics ([#181](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/181)) ([40a2019](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/40a2019b59e8fdc240b65da9537145b87e524de1))
* File with extensions for other files should not be set to hlasm ([#173](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/173)) ([fc49775](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/fc497751149b72c2b2a318b618928adabcd18032))
* Fix HLASM Listing highlighting on lines with trimmed whitespace ([#199](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/199)) ([4262e27](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/4262e27861eef1aa7ed87fd8bcac898e6b679bff))
* Incorrect relative immediate operand validation (fixes [#177](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/177)) ([614c86e](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/614c86e844fa7ff47c86b9ceec52f484e0d672aa))
* Infinite loop during lookahead processing when model statement is located in copybook ([#189](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/189)) ([176de31](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/176de31a040fe67e59a8abcf2852a63a3a1af5c1))
* Language server crashes while trying to list inaccessible directory ([60db271](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/60db2715b7a5356a102d80bb22a3add834b00a53))
* Lookahead mode does not work correctly when triggered from AINSERTed code ([#175](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/175)) ([f7143c8](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/f7143c8326f5ddbdea46827e7ccd2a700c841a33))
* Operands of dynamically generated statements may be incorrectly parsed ([#185](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/185)) ([1a9127e](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/1a9127e509cd0cf4137a3bc4a6d92ab742cb616f))
* Preserve mixed-case labels on macro calls ([#165](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/165)) ([d8545fe](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/d8545fee66cf2987d9cdf179529df4d7987d9c6e)), closes [eclipse/che-che4z-lsp-for-hlasm#155](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/155)
* Process END instruction ([#184](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/184)) ([2d5ad75](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/2d5ad7593f78a50483d528568996a4e6af701581))
* Remove ALIAS operand parsing limitation ([#178](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/178)) ([480e602](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/480e602c30fc6cf0cddcd017ee4abe66d75fc043)), closes [#157](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/157)
* Various fixes ([#166](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/166)) ([200b769](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/200b769e5da0082be264971fdc4a916d63426211))
* Write the error name directly to method name of telemetry event ([#200](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/200)) ([6dd6b9f](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/6dd6b9f91cfb10791809c5e4b833a4e1a78e693c))

## [0.14.0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/compare/0.13.0...0.14.0) (2021-08-18)


### Features

* Implement the CCW* instructions ([#154](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/154)) ([f750bcc](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/f750bccc051abea4b1141a71720df109f0a3a670))
* Minimal DB2 preprocessor emulator ([#140](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/140)) ([77275dd](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/77275dd91e8a8ba40ca8a60bd2d47435ccf39ff5))
* Support for complex SQL types ([#146](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/146)) ([3e85b98](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/3e85b98b476eb0b66566a9e729d540fe8fbb8f36))


### Fixes

* AREAD/AINSERT support in macros called from copybooks ([#138](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/138)) ([bdc3718](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/bdc371894730eef021f08f8cadb45f07d622ba6e))
* DB2 LOB locator and file variables are not processed correctly ([20a6fba](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/20a6fba747ca19682806eeadeef1ced851b881ea))
* Dependency files caching ([#129](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/129)) ([2541b7a](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/2541b7a18070e10071b5586827659f6695d54755))
* EXLR flagged as error by plugin ([#121](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/121)) ([e097903](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/e097903698dcafac65c3ccc01faea6a78e9cc85d))
* Inline macros overwriting external definition stored in macro cache ([#148](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/148)) ([93107b3](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/93107b315298ce9b3f5652f28b8ad7a3b8a7dbf3))
* language server crashes while evaluating conditional assembly statements ([#139](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/139)) ([249e85d](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/249e85d2da17caca532d8fadd8517f215a4dbfd4))
* Remove (no longer supported) experimental flags when running WASM server variant on V8 version 9 and newer. ([1cabd76](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/1cabd76999fdffa7a8e700e943813c9f41ac3431))
* Syntax errors reported in bilingual macros ([#152](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/152)) ([a8b1201](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/a8b1201c7cddef5c9d5b5a00b6ae71d3a6e75641)), closes [#144](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/144)
* Various small fixes ([#149](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/149)) ([c1a6896](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/c1a6896ed799efa91f9cc7ff10f5f5d6b7981de3)), closes [eclipse/che-che4z-lsp-for-hlasm#143](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/143) [eclipse/che-che4z-lsp-for-hlasm#142](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/142)
* Various small fixes ([#150](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/150)) ([36fdbda](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/36fdbda1ca93e5a3d4ca173f6253f2b04c6ed53f))


### Other changes

* changelog + readme for preprocessors ([70caf82](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/70caf8292ca64b82f94533de3b83002f42a20168))
* Introduce a  mock usable throughout the parser library tests ([#145](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/145)) ([547948e](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/547948e1a82f701338aa42de57c7f4e18f58c408))
* Parser reports all its diagnostics using single channel ([#141](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/141)) ([656caf3](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/656caf3a21d231713b7d66a8cdbf26091a4a20ac))
* Split complex classes ([#134](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/134)) ([78ca7e7](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/78ca7e75189df5123379110b2e1bb20b7552143e))

## [0.13.0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/compare/0.12.0...0.13.0) (2021-06-01)


### Features

* AREAD support ([#125](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/125)) ([052c844](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/052c84463cdd2bb390877ae682abca385a4c05ca))
* Provide users ability to use compiler option SYSPARM ([#108](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/108)) ([ccb3a0a](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/ccb3a0a38ef562cbee9ae2348ef278d352ced74a))
* Support for macro file extensions ([#117](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/117)) ([d5b21d2](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/d5b21d2f37b815da3a6bc9dfaa3da3ee5ad87c68))
* Support missing instruction ([#113](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/113)) ([ec547cf](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/ec547cf29a071167e6acffed3eac1900daf8df66))
* Support running under Node.js ([#111](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/111)) ([dc0c47b](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/dc0c47bd756e293c0724c6e4a25b85a632aa3f46))
* UI enhancement for the WASM server variant ([#120](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/120)) ([2d73b0d](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/2d73b0db89b522e8b53fc39b0ed1fc526edd5fe3)), closes [eclipse/che-che4z-lsp-for-hlasm#122](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/122)


### Fixes

* conversion of an empty string to a number ([#119](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/119)) ([b5e6989](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/b5e69896f9daed795e3c0ea62c89dd5eeec3a1bd))
* Fix crash when hovering over non-existing instruction ([3fbb22e](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/3fbb22ee1837fc38ef2709cb5252c447b67e60b2))
* Refactor the way of collecting LSP information ([#110](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/110)) ([d767b6d](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/d767b6d2860fbe90e25d9d5e9e876a1fa8fff4cd))


### Other changes

* Coverity scan ([#123](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/123)) ([daa662e](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/daa662ee8e02f1e5fd41f912c2ab099225faee1b))
* DAP via LSP messages ([#109](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/109)) ([044acad](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/044acad3e5a57610d19492f79b90d759bb7d10d4))
* Implement suggestions from tools ([#118](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/118)) ([b6f231b](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/b6f231b01a8862eb415f6f00a2105fccc5a78934))
* Remove mention of unimplemented PROFILE option ([5f4ce87](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/5f4ce87e653e700331455c04c9307297d633910a))
* Statement refactor ([#93](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/93)) ([0f41701](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/0f41701b3a23405d0e4e63f66700efe2eac22c46))

## [0.12.0](https://github.com/eclipse/che-che4z-lsp-for-hlasm/compare/0.11.1...0.12.0) (2020-12-18)


### Features

* Diagnostics suppression for files with no pgm_conf configuration ([#89](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/89)) ([c287ff1](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/c287ff13e166edf7a191fbf5482a125648b8986a))


### Other changes

* Add example workspace ([#44](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/44)) ([774bb1d](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/774bb1dde75bdaf6456a6bf0b40bceca76760053))
* Add thread sanitizer to CI ([#78](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/78)) ([225b74f](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/225b74f408a29b1288a4f394cd79b23b31dc5295))
* automatic release mechanism ([#53](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/53)) ([0abf1d8](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/0abf1d8a35d41c9095cd57135f789a0e7b4e5036))
* fix obtaining version in theia tests ([23594bb](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/23594bb86207fa70f67ab5bb3e3f8df63fa7722f))
* fix semantic-release not creating pre-release ([9c65e12](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/9c65e12ae598463d994874fa88d5da9ecc511639))
* fix set-env command that is no longer supported ([#95](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/95)) ([c7d061c](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/c7d061cfa0bab6e9c1715c0ec7b2d265f3ff3e71))
* Improve test coverage of Language Server component ([#61](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/61)) ([6c67153](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/6c671533c28e180ddc369b74966af5bbedcc0b1e))
* Integration tests ([#51](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/51)) ([4b3d153](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/4b3d1531014a2601848168df6d52af2eb7e5890c))
* Refactor of attribute lookahead processing ([#84](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/84)) ([ce8e59d](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/ce8e59df8415935be2e007459d62ecd23ee0adad))
* Server test deadlock fix ([#83](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/83)) ([9cc2dfc](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/9cc2dfc33404671df2dea213e146f33ce6ee4e3b))
* TI review of wiki pages and updates to readme files ([#56](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/56)) ([e66ea4a](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/e66ea4a823485335ea58fda14abf024743b95852))
* Update link of join slack badge ([1ec6bfc](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/1ec6bfc7fbf239a8474659c1362e8c3fc642b753))
* wiki inside repository ([#50](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/50)) ([1be2a95](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/1be2a9594e050b49f26c30ea9b3a625cd5942825))


### Fixes

* Conditional assembly expressions ([#65](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/65)) ([99c45ee](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/99c45ee0a45d4b54b4c043e79afb92b73edee935))
* False positives ([#86](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/86)) ([34f3a5e](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/34f3a5ec52e38f3d21471bfb49962e991d348f99))
* Fixed little things in suppression section ([#99](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/99)) ([9374153](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/937415380644e2f26f51ff2fd6b7aa4ab462da41))
* Instruction completion issue ([#64](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/64)) ([8f31888](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/8f3188848a56dd6a1e90aa1a2da107dbeed4e8ab))
* Lexer infinite loop fix ([#85](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/85)) ([027b9f9](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/027b9f99a28ad5a60c9b0f0ed9762a2d643f8610))
* Show ampersands in names of variable symbols and macro parameters in macro tracer ([#79](https://github.com/eclipse/che-che4z-lsp-for-hlasm/issues/79)) ([2c2338c](https://github.com/eclipse/che-che4z-lsp-for-hlasm/commit/2c2338c89fb1f7aeb34fdb3315cf102a66ad3ddd))

