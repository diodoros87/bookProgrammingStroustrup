class Parsing_Test {
   static void parseIncorrectString(const string & integer_name, Integer integer, const string & incorrect_string) {
      try {
         integer.parse(incorrect_string);
         assert(false);
      } catch (const invalid_argument & e) {
         cerr << __func__ << " exception: " << e.what() << endl;
      }
   }

   static void parseString(String integer_name, Integer integer, String string) {
      integer.parse(string);
      System.out.printf("%s integer after parsing of \'%s\': %s%n", integer_name, string, integer);
   }

   public static void testOfParsing(String integer_name, Integer integer) {
      System.out.printf("%n TESTS OF PARSING STRING :%n");

      parseString(integer_name, integer, "12345");
      assert(integer.toString().equals("+12345"));

      parseString(integer_name, integer, "-1234565");
      assert(integer.toString().equals("-1234565"));

      parseString(integer_name, integer, "+45");
      assert(integer.toString().equals("+45"));

      parseString(integer_name, integer, "0");
      assert(integer.toString().equals("0"));

      parseIncorrectString(integer_name, integer, "E123");
      assert(integer.toString().equals("0"));

      parseIncorrectString(integer_name, integer, "p");
      assert(integer.toString().equals("0"));

      parseIncorrectString(integer_name, integer, "6p");
      assert(integer.toString().equals("0"));

      parseString(integer_name, integer, "10");
      assert(integer.toString().equals("+10"));

      parseString(integer_name, integer, "345");
      assert(integer.toString().equals("+345"));

      parseIncorrectString(integer_name, integer, "0p");
      assert(integer.toString().equals("+345"));

      parseIncorrectString(integer_name, integer, "++7");
      assert(integer.toString().equals("+345"));

      parseIncorrectString(integer_name, integer, "");
      assert(integer.toString().equals("+345"));

      parseIncorrectString(integer_name, integer, " ");
      assert(integer.toString().equals("+345"));

      parseIncorrectString(integer_name, integer, "8 ");
      assert(integer.toString().equals("+345"));

      parseIncorrectString(integer_name, integer, "-0");
      assert(integer.toString().equals("+345"));

      parseString(integer_name, integer, "000");
      assert(integer.toString().equals("0"));

      parseString(integer_name, integer, "088");
      assert(integer.toString().equals("+88"));

      parseString(integer_name, integer, "-02");
      assert(integer.toString().equals("-2"));

      parseIncorrectString(integer_name, integer, "844444444444444444444444444444444444444444444444444444444444444444444444444");
      assert(integer.toString().equals("-2"));

      System.out.printf("%n ------------------------%n");
   }
}
