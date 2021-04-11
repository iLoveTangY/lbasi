// 用 ts 重新实现一遍 calc
enum TokenType {
    INTEGER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    EOF,
}

class Token {
    public constructor(public token_type: TokenType, public value: string | number) {}
}

// 支持四则运算的词法分析器
class Lexer {
    private text: string;
    private pos : number;
    private current_char : string;
    private number_reg: RegExp = new RegExp("^[0-9]$");

    public constructor(text: string) {
        this.text = text;
        this.pos = 0;
        this.current_char = this.text.charAt(this.pos);
    }

    private throwError(msg: string) {
        throw new SyntaxError(msg);
    }

    private currentCharIsDigit() {
        return this.number_reg.test(this.current_char);
    }

    private advance() {
         this.pos += 1;
        if (this.pos < this.text.length) {
            this.current_char = this.text.charAt(this.pos);
        } else {
            this.current_char = null;
        }
    }

    private getNumber() : number {
        let start_pos = this.pos;
        while (null != this.current_char && this.currentCharIsDigit()) {
            this.advance();
        }
        let result = Number(this.text.substring(start_pos, this.pos));
        return result;
    }

    private skipWhiteSpace() {
        while (this.current_char != null && this.current_char == " ") {
            this.advance();
        }
    }

    public getNextToken() : Token {
        while (this.current_char != null) {
            if (this.current_char == " ") {
                this.skipWhiteSpace();
            } else if (this.currentCharIsDigit()) {
                return new Token(TokenType.INTEGER, this.getNumber());
            } else if (this.current_char == "+") {
                this.advance();
                return new Token(TokenType.PLUS, "+");
            } else if (this.current_char == "-") {
                this.advance();
                return new Token(TokenType.MINUS, "-");
            }  else if (this.current_char == "*") {
                this.advance();
                return new Token(TokenType.MUL, "*");
            } else if (this.current_char == "/") {
                this.advance();
                return new Token(TokenType.DIV, "/");
            } else {
                this.throwError("term should be number or plus or minus");
            }
        }
        return new Token(TokenType.EOF, "");
    }
}

class Interpreter {
    private lexer : Lexer;
    private current_token : Token;

    public constructor(lexer: Lexer) {
        this.lexer = lexer;
        this.current_token = this.lexer.getNextToken();
    }

    private throwError(msg: string) {
        throw new SyntaxError(msg);
    }

    private assertTokenType(type: TokenType) {
        if (this.current_token.token_type == type) {
            this.current_token = this.lexer.getNextToken();
            return;
        }
        this.throwError(`current token type should be ${type}`);
    }

    private factor(): number {
        const token = this.current_token;
        this.assertTokenType(TokenType.INTEGER);
        return token.value as number;
    }

    private term(): number {
        let result = this.factor();
        let token_type = this.current_token.token_type;
        while (token_type == TokenType.MUL || token_type == TokenType.DIV) {
            if (token_type == TokenType.MUL) {
                this.assertTokenType(TokenType.MUL);
                result *= this.factor();
            } else if (token_type == TokenType.DIV) {
                this.assertTokenType(TokenType.DIV);
                result /= this.factor();
            } else {
                this.throwError(`current char should be '*' or '/' instead of ${this.current_token.value}`);
            }
            token_type = this.current_token.token_type;
        }
        return result;
    }

    // 四则运算计算器的上下文无关语法如下：
    // expr: term ((PLUS | MINUS) term)*
    // term: factor ((MUL | DIV) factor)*
    // factor: INTEGER
    public expr(): number {
        let result = this.term();
        let token_type = this.current_token.token_type;
        while (token_type == TokenType.PLUS || token_type == TokenType.MINUS) {
            if (token_type == TokenType.PLUS) {
                this.assertTokenType(TokenType.PLUS);
                result += this.term();
            } else if (token_type == TokenType.MINUS) {
                this.assertTokenType(TokenType.MINUS);
                result -= this.term();
            } else {
                this.throwError(`current char should be '+' or '-' instead of ${this.current_token.value}`);
            }
            token_type = this.current_token.token_type;
        }
        return result;
    }

}

const test_string_1: string = "1 + 2 * 3";
const test_string_2: string = "1 + 2 / 2 + 3 - 5";
const test_string_3: string = "12 + 32 - 3 + 321"

console.log(`${test_string_1} = ${new Interpreter(new Lexer(test_string_1)).expr()}`);
console.log(`${test_string_2} = ${new Interpreter(new Lexer(test_string_2)).expr()}`);
console.log(`${test_string_3} = ${new Interpreter(new Lexer(test_string_3)).expr()}`);
