Q1: Draw a syntax diagram for arithmetic expressions that contain only multiplication and division, for example “7 * 4 / 2 * 3”. 
```mermaid
stateDiagram-v2
[*] --> term1
term1 --> nop
nop --> *
nop --> /
* --> term2
/ --> term2
term2 --> nop 
term2 --> [*]
```