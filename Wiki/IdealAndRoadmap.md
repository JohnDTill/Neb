# Ideal

Neb aims to parse any valid mathematical syntax in unicode strings or the MathBran serialized format. It aims to mimic conventional mathematical syntax, despite some ambiguity in the source language. Neb has the *nebulous* philosophy of parsing as much valid mathematical syntax as possible, regardless of how practical it is to interpret or process the parse tree in a later stage. This division between parsing and interpretation should ultimately be helpful to develop a robust technology stack.

### Roadmap

The parser shows promise and is shaping up well. Many constructs are supported, and implicit multiplication is working well. There are quite a few notations which have yet to be supported, such as second derivatives. It is currently unclear how Neb will be support the interleaving of equations and programming statements, e.g. allowing control flow or defining "functions" as a sequence of imperative statements. Could do some more work with code gen- anything to keep the parser size down since it is already >1000 lines. Perhaps Neb should include some form of type checking?
