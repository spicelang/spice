# Contributing
Spice is a community-driven project. This means that we are open for feature requests or suggestions for improvement.
Apart from that, feel free to fork this project and send us some code contributions via pull request.

In every case please read the [Code of Conduct](https://github.com/spicelang/spice/blob/main/CODE_OF_CONDUCT.md) before contributing.

## Code style
Before sending in code contributions, please read the [Coding Style Guide](https://github.com/spicelang/spice/blob/main/STYLE_GUIDE.md). It documents the formatting, naming and structural conventions used throughout the compiler (C++) and standard library (Spice), so that contributions stay consistent.

## Security issues
Please do **not** open issues regrading security on GitHub! Please read our [Security Policy](https://github.com/spicelang/spice/blob/main/SECURITY.md) and contact us via non-public ways, e.g. per mail: [contact@chillibits.com](mailto:contact@chillibits.com).

## Feature request and bugs
If you want to contribute by reporting a bug (except a security issue) or you want to send in a feature request, please navigate to the [issues section](https://github.com/spicelang/spice/issues). Issues that are easy to work on and appropriate for beginners are marked as 'good first issue'. Feel free to claim one of those issues by commenting that you want to do so. After we assign you to the issue, you can start working on the problem / feature request. Please open a pull request when you're done with the work and please only work on one issue at once.
## Documentation examples
The CI compiles every complete program (every `spice` code block with a `f<int> main(` function) in the documentation
with the compiler under test. To check the examples locally, run:

```sh
python tools/check_doc_examples.py --compiler build/spice
```

If an example is not meant to compile on its own, put a marker comment in front of its code block:

- `<!-- doc-test: skip <reason> -->` skips the code block, e.g. because it continues an earlier snippet or imports the
  source file of another code block.
- `<!-- doc-test: expect-error <message> -->` expects the compilation to fail with an output, that contains the message,
  e.g. because the code block demonstrates a compile error.
