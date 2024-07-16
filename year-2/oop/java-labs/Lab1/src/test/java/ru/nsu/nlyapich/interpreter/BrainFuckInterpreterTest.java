package ru.nsu.nlyapich.interpreter;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import ru.nsu.nlyapich.App;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

public class BrainFuckInterpreterTest {
    @Test
    public void testRun() throws IOException {
        String testCase[][] = {
            {
                "examples/english_alphabet.bf",

                "Aa Bb Cc Dd Ee Ff \n" +
                "Gg Hh Ii Jj Kk Ll \n" +
                "Mm Nn Oo Pp Qq Rr \n" +
                "Ss Tt Uu Vv Ww Xx \n" +
                "Yy Zz "
            },
            {
                "examples/hello.bf",
                "Hello World!\n"
            },
            {
                "examples/hello2.bf",
                "Hello World!\n"
            },
            {
                "examples/example1.bf",
                ">>+++++++>>++>>++++>>+++++++>>+>>++++>>+>>+++>>+>>+++++>>+>>++>>+>>++++++>>++>>++++>>+++++++>>+>>+++++>>++>>+>>+>>++++>>+++++++>>+>>+++++>>+>>+>>+>>++++>>+++++++>>+>>+++++>>++++++++++++++>>+>>+>>++++>>+++++++>>+>>+++++>>++>>+>>+>>++++>>+++++++>>+>>+++++>>+++++++++++++++++++++++++++++>>+>>+>>++++>>+++++++>>+>>+++++>>++>>+>>+>>+++++>>+>>++++++>>+>>++>>+>>++++++>>+>>++>>+>>++++++>>+>>++>>+>>++++++>>+>>++>>+>>++++++>>+>>++>>+>>++++++>>+>>++>>+>>++++++>>++>>++++>>+++++++>>+>>+++++>>+++++++>>+>>+++++>>+>>+>>+>>++++>>+>>++>>+>>++++++>>+>>+++++>>+++++++>>+>>++++>>+>>+>>++>>+++++>>+>>+++>>+>>++++>>+>>++>>+>>++++++>>+>>+++++>>+++++++++++++++++++>>++>>++>>+++>>++>>+>>++>>++++>>+++++++>>++>>+++++>>++++++++++>>+>>++>>++++>>+>>++>>+>>++++++>>++++++>>+>>+>>+++++>>+>>++++++>>++>>+++++>>+++++++>>++>>++++>>+>>++++++[<<]>>[>++++++[-<<++++++++++>>]<<++..------------------->[-<.>>+<]>[-<+>]>]<<[-[-[-[-[-[-[>++>]<+++++++++++++++++++++++++++++>]<++>]<++++++++++++++>]<+>]<++>]<<[->.<]<<]"
            }
        };

        String configFileName = App.class.getClassLoader().getResource("examples/commands.properties").getPath();

        for (int i = 0; i < testCase.length; ++i){
            String codeFileName = App.class.getClassLoader().getResource(testCase[i][0]).getPath();
            BrainFuckInterpreter bf = new BrainFuckInterpreter(codeFileName, configFileName);
            ByteArrayOutputStream ostream = new ByteArrayOutputStream();
            bf.run(ostream, null);
            String actual = ostream.toString();
            String expected = testCase[i][1];
            Assertions.assertEquals(expected, actual);

        }
    }

    @Test
    public void testException() throws IOException {
        String codeFileName = App.class.getClassLoader().getResource("examples/exception_test/code.bf").getPath();
        String configFileName = App.class.getClassLoader().getResource("examples/exception_test/bad_commands.properties").getPath();

        Assertions.assertThrows(IOException.class, () -> {
            BrainFuckInterpreter bf = new BrainFuckInterpreter(codeFileName, configFileName);
        });
    }
}
