# Neb

![MathLimerick.png](MathLimerick.png?raw=true "And some people think math is a chore!")

## Parser with a nebulous purpose

_"What is my purpose? What am I? Where am I going? Who can I serve along the way?"_

Our parser does not know the answers to any of these questions. It's just going to focus on what it knows how to do: parsing. Neb parses mathematical syntax in unicode or the [MathBran](https://github.com/JohnDTill/MathBran) serialized format. Neb has the nebulous philosophy of parsing as much valid mathematical syntax as possible, regardless of how practical it is to interpret or process the parse tree in a later stage. This division between parsing and interpretation should ultimately be helpful to develop a robust technology stack.

The parser is implemented with recursive descent, and uses several LibreOffice spreadsheets to perform code generation of token and AST node definitions.

![DiffusionEq.png](DiffusionEq.png?raw=true "Good luck interpreting an equation like this, but we can parse it!")

![LinearControlEq.png](LinearControlEq.png?raw=true "Matrices too!")


## Prerequisites

* Qt (https://www.qt.io/download)
* Optional: YAWYSIWYGEE ([https://github.com/JohnDTill/YAWYSIWYGEE](https://github.com/JohnDTill/YAWYSIWYGEE))

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
