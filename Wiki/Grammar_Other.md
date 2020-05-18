## Shorthand

c and s default meanings, or possible to overload

```
The rotation for roll-pitch-yaw (ϕ,θ,ψ) is:
R = ⁜⊞⏴3⏵⏴3⏵⏴cθcψ⏵⏴sϕsθcψ-cϕsψ⏵⏴cϕsθcψ+sϕsψ⏵⏴cθsψ⏵⏴sϕsθsψ+cϕcψ⏵⏴cϕsθsψ-sϕcψ⏵⏴-sθ⏵⏴sϕcθ⏵⏴cϕcθ⏵
```

This would work with single-char identifiers, implicit function calls, and "sin" bound to 's' and "cos" to 'c'.

## Recoverability

I struggle to know how reversible the parse tree should be. For instance, if you have a pair of statements `a = 1, b = 2`, should the parse tree record that the statements are separated by a comma, or is that level of detail overkill? Or should the parse tree record the difference between a fraction `⁜f⏴1⏵⏴2⏵` and a division `1/2`?
