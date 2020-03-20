# Newlines

Generally, newlines are used to terminate statements. The following code has two distinct equality statements:
```
2y + 3x = 8
-y + 2x = 3
```
However, it is often desirable to split long expressions over multiple lines. A single newline is allowed after a binary operator:
```
E ≡
mc²
```
A newline is not permitted before a binary operator, because that would lead to ambiguities such as being able to parse the first example as `2y + 3x = 8 - y + 2x = 3`
