package ru.nsu.nlyapich.util;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

public class ReaderLoopPosition {
    private static final Logger logger = LogManager.getLogger(ReaderLoopPosition.class);
    public static Map<Integer, Integer> read(InputStream codeStream, char openingLoop, char closingLoop) throws IOException {
        logger.info("Reading file for loops");

        Map<Integer, Integer> map = new HashMap<>();

        int countBytes = codeStream.available();
        byte[] bytes = new byte[countBytes];
        codeStream.read(bytes);

        Stack<Integer> stack = new Stack<>();
        for (int i = 0; i < bytes.length; ++i) {
            char curSymbol = (char) bytes[i];
            if (curSymbol == openingLoop) {
                stack.push(i);
            }
            if (curSymbol == closingLoop) {
                int lastOpened = stack.pop();
                map.put(i, lastOpened);
                map.put(lastOpened, i);
            }
        }

        return map;
    }
}