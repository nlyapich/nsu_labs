package ru.nsu.nlyapich.util;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import ru.nsu.nlyapich.App;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class ReaderLoopPositionTest {
    boolean equalMaps(Map<Integer, Integer> m1, Map<Integer, Integer> m2) {
        if (m1.size() != m2.size()) {
            return false;
        }
        for (Integer key: m1.keySet()) {
            if (!m1.get(key).equals(m2.get(key))) {
                return false;
            }
        }
        return true;
    }
    @Test
    void testRead() throws IOException {
        char startLoop = '[';
        char endLoop = ']';
        String codeFileName = App.class.getClassLoader().getResource("examples/example1.bf").getPath();

        FileInputStream codeStream = new FileInputStream(codeFileName);
        Map<Integer, Integer> blocks = ReaderLoopPosition.read(codeStream, startLoop, endLoop);
        codeStream.close();

        Integer testCase[][] = {
                {896,935},
                {961,890},
                {898,903},
                {966,888},
                {839,823},
                {969,974},
                {977,886},
                {866,874},
                {809,812},
                {876,881},
                {940,894},
                {815,883},
                {892,957},
        };
        Map<Integer, Integer> expectedBlocks = new HashMap<Integer, Integer>();
        for (int i = 0; i < testCase.length; ++i){
            expectedBlocks.put(testCase[i][0], testCase[i][1]);
            expectedBlocks.put(testCase[i][1], testCase[i][0]);
        }

        Assertions.assertTrue(equalMaps(expectedBlocks, blocks));
    }
}
