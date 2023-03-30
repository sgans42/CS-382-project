// See https://aka.ms/new-console-template for more information
using System;

namespace LexicalAnalyzerAndParser
{
    public class test
    {
        private string input;
    }

    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
        }
    }

    public class LexicalAnalyzer
    {
        private string input;
        private char nextChar;
        private string lexeme;
        private Token nextToken;

        public LexicalAnalyzer(string input) { }

        private void lex() { }

        private void getChar() { }

        private void addChar() { }

        private void lookup(char ch) { }

        private void getNonBlank() { }

        public List<Token> Tokenize() { }
    }

    public class Parser
    {
        private string input;
        private Token lookahead;
        private List<Token> tokens;
        private int currentTokenIndex;

        public Parser(string input) { }

        private void match(TokenType expectedTokenType) { }

        private TreeNode factor() { }

        private TreeNode term() { }

        private TreeNode expr() { }

        public TreeNode GenerateParseTree() { }

        private void PrintParseTree(TreeNode node, string indent) { }

        public void PrintParseTree() { }
    }
}
