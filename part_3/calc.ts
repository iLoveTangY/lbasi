// 用 ts 重新实现一遍 calc
enum TokenType {
    INTEGER,
    PLUS,
    MINUS
}

class Token {
    public constructor(public token_type: TokenType, public value: string | number) {}
}

class Interpreter {
    private text: string;
    private current_token : Token;
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

    private getNextToken() : Token {
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
            } else {
                this.throwError("term should be number or plus or minus");
            }
        }
        return null;
    }

    private assertTokenType(type: TokenType) {
        if (this.current_token.token_type == type) {
            this.current_token = this.getNextToken();
            return;
        }
        this.throwError(`current token should be ${type}`);
    }

    private getTerm() : number {
        let token = this.current_token;
        this.assertTokenType(TokenType.INTEGER);
        return token.value as number;
    }

    public interpreter() : number{
        this.current_token = this.getNextToken();
        let result = this.current_token.value as number;
        this.assertTokenType(TokenType.INTEGER);

        while (this.current_token != null && (this.current_token.token_type == TokenType.PLUS || this.current_token.token_type == TokenType.MINUS)) {
            if (this.current_token.token_type == TokenType.PLUS) {
                this.assertTokenType(TokenType.PLUS);
                result += this.getTerm();
            } else if (this.current_token.token_type == TokenType.MINUS) {
                this.assertTokenType(TokenType.MINUS);
                result -= this.getTerm();
            } else {
                this.throwError("Only support plus and minus");
            }
        }

        return result;
    }
}

const test_string_1: string = "1 + 2";
const test_string_2: string = "1 + 2 + 3 - 5";
const test_string_3: string = "12 + 32 - 3 + 321"

console.log(`${test_string_1} = ${new Interpreter(test_string_1).interpreter()}`);
console.log(`${test_string_2} = ${new Interpreter(test_string_2).interpreter()}`);
console.log(`${test_string_3} = ${new Interpreter(test_string_3).interpreter()}`);
