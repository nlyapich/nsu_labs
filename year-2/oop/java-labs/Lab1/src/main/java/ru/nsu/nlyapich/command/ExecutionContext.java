package ru.nsu.nlyapich.command;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.Map;

public class ExecutionContext {

    private static final Logger logger = LogManager.getLogger(ExecutionContext.class);
    public Memory _memory;
    private Map<Integer, Integer> _blocks;

    public int curPos;

    public ExecutionContext(int size, Map<Integer, Integer> blocks) {
        logger.info("Creating ExecutionContext");
        _blocks = blocks;
        _memory = new Memory(size);
        curPos = 0;
    }

    public int getLoopPairPos(int curPos) { return _blocks.get(curPos); }

    public Memory getMemory() { return _memory; }
}
