# SLR (Simple LR)
## LR 
LR parsing is one type of bottom up parsing. It is constructed using an automatic parser generator .
## SLR
SLR parser is a type of LR parser with small parse tables and a relatively simple parser generator algorithm. As with other types of LR(1) parser, an SLR parser is quite efficient at finding the single correct bottom-up parse in a single left-to-right scan over the input stream, without guesswork or backtracking.
## SLR Grammar
A Grammar is said to be SLR if and only if, for each and every state 's' in the SLR automaton, none of the following conditions are violated:
1. For any reducible rule 'A → a • Xb' in state s (where X is some terminal), there must not exist some irreducible rule, 'B → a •' in the same state 's' such that the follow set of B contains the terminal X. In more formal terms, the intersection of set containing the terminal X and the follow set of B must be empty. Violation of this rule is a Shift-Reduce Conflict.
2. For any two complete items 'A → a •' and 'B → b •' in 's', Follow(A) and Follow(B) are disjoint (their intersection is the empty set). Violation of this rule is a Reduce-Reduce Conflict.
# Installation
# Usage
# License
This project is licensed under the GNU GPL v3.0 License - see the [LICENSE](https://github.com/AbdelMoumene-Hadfi/SLR/blob/master/LICENSE) file for details.
