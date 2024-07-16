package ru.nsu.nlyapich.view;

import ru.nsu.nlyapich.Display;
import ru.nsu.nlyapich.controller.ControllerSessions;
import ru.nsu.nlyapich.model.*;

import java.awt.*;

public class View implements Runnable{

    public static final int		WIDTH			= 1000;
    public static final int		HEIGHT			= 700;
    public static final String	TITLE			= "Game";
    public static final int		CLEAR_COLOR		= 0xff000000;
    public static final int		NUM_BUFFERS		= 3;

    public static final float	UPDATE_RATE		= 30.0f;

    public static final long	SECOND	= 1000000000l;
    public static final float	UPDATE_INTERVAL	= SECOND / UPDATE_RATE;

    private boolean				running;
    private Thread				gameThread;
    private Graphics2D graphics;
    private ViewWorld viewWorld;
    private ViewMenu viewMenu;
    private ViewHelp viewHelp;
    private ViewPause viewPause;
    private ViewEndGame viewEndGame;
    private ViewWinGame viewWinGame;
    private ControllerSessions controller;
    public View(ModelWorld world, ModelMenu menu, ModelHelp help, ModelPause pause, ModelEndGame endGame, ModelWinGame winGame, ControllerSessions controller){
        viewWorld = new ViewWorld(world, HEIGHT, WIDTH);
        viewMenu = new ViewMenu(menu, HEIGHT, WIDTH);
        viewHelp = new ViewHelp(help, HEIGHT, WIDTH);
        viewPause = new ViewPause(pause, HEIGHT, WIDTH);
        viewEndGame = new ViewEndGame(endGame, HEIGHT, WIDTH);
        viewWinGame = new ViewWinGame(winGame, HEIGHT, WIDTH);

        this.controller = controller;

        running = false;

        Display.create(WIDTH, HEIGHT, TITLE, CLEAR_COLOR, NUM_BUFFERS);
        graphics = Display.getGraphics();
    }

    public synchronized void start(){
        if (running){
            return;
        }

        running = true;
        gameThread = new Thread(this);
        gameThread.start();
    }

    public synchronized void stop(){
        if (!running)
            return;

        running = false;

        try {
            gameThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        cleanUp();
    }

    public void render(){
        Display.clear();

        ViewerModel curViewer;
        switch (controller.getCurTypeSession()){
            case SESSION_WORLD -> {
                curViewer = viewWorld;
            }
            case SESSION_MENU -> {
                curViewer = viewMenu;
            }
            case SESSION_HELP -> {
                curViewer = viewHelp;
            }
            case SESSION_PAUSE -> {
                curViewer = viewPause;
            }
            case SESSION_END_GAME -> {
                curViewer = viewEndGame;
            }
            case SESSION_WIN -> {
                curViewer = viewWinGame;
            }
            default -> {
                curViewer = viewMenu;
            }
        }
        curViewer.render(graphics);

        Display.swapBuffers();
    }

    @Override
    public void run() {
        int fps = 0;

        long count = 0;

        float delta = 0;

        long lastTime = System.nanoTime();
        while (running) {
            long now = System.nanoTime();
            long elapsedTime = now - lastTime;
            lastTime = now;

            count += elapsedTime;

            delta += (elapsedTime / UPDATE_INTERVAL);
            while (delta > 1) {
                render();
                fps++;
                delta--;
            }

            if (count >= SECOND) {
                Display.setTitle(TITLE + " || Fps: " + fps);
                fps = 0;
                count = 0;
            }

        }
    }

    private void cleanUp() {
        Display.destroy();
    }
}
