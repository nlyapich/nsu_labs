package ru.nsu.nlyapich.model;

import ru.nsu.nlyapich.Command;
import ru.nsu.nlyapich.controller.StatusSession;

import java.awt.geom.Point2D;
import java.util.*;

public class ModelWorld implements Runnable {
    public static final long	SECOND	= 1000000000l;
    public static final int		WIDTH			= 1000;
    public static final int		HEIGHT			= 700;

    public static final double MOVE = 5;
    public static final double ROTATION_ANGLE = 5;

    private Player player;
    private List<Enemy> enemies;
    private StatusSession statusSession;

    private int[][] map;

    private int sizeBlock;

    private boolean running;

    private Thread gameThread;

    public ModelWorld(){
        running = false;
    }

    @Override
    public void run() {
        long lastTime = System.nanoTime();

        long count = 0;

        while (running && (statusSession != StatusSession.GAME_OVER) && (statusSession != StatusSession.PAUSE) && (statusSession != StatusSession.WIN)) {
            long now = System.nanoTime();
            long elapsedTime = now - lastTime;
            lastTime = now;

            count += elapsedTime;

            if (count >= SECOND) {
                count = 0;
                for (int i = 0; i < 2; ++i) {
                    for (int j = 0; j < enemies.size(); ++j) {
                        if (statusSession == StatusSession.PAUSE){
                            break;
                        }

                        Enemy curEnemy = enemies.get(j);
                        double ex = curEnemy.x;
                        double ey = curEnemy.y;
                        double px = player.x;
                        double py = player.y;

                        double distance = Point2D.Double.distance(ex, ey, px, py);
                        if (distance > 10) {
                            if(ex>px){ curEnemy.x -= 5;}
                            if(ex<px){ curEnemy.x += 5;}
                            if(ey>py){ curEnemy.y -= 5;}
                            if(ey<py){ curEnemy.y += 5;}
                        } else {
                            player.decreaseHealth(curEnemy.getPower());
                            if (player.isDead()) {
                                statusSession = StatusSession.GAME_OVER;
                                break;
                            }
                        }
                    }
                    if ((statusSession == StatusSession.GAME_OVER) || (statusSession == StatusSession.PAUSE)){
                        break;
                    }
                    try {
                        Thread.sleep(300);
                    } catch (InterruptedException e) {
                        throw new RuntimeException(e);
                    }
                }
            }
        }
    }

    public void start(){
        player = new Player(400, 400, 240, 30, 100);
        enemies = new ArrayList<> (Arrays.asList(new Enemy[]{
                        new Enemy(300, 300, 10, 100),
                        new Enemy(310, 310, 10, 100)})
        );

        loadMap();
        statusSession = StatusSession.ACTIVE;

        if (running){
            return;
        }

        running = true;

        gameThread = new Thread((Runnable) this);
        gameThread.start();
    }

    private void loadMap(){
        map = new int[][] {
                {1,1,1,1,1,1,1,1,1,1,1,1,1},
                {1,4,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,2,2,2,0,0,0,0,0,0,1},
                {1,0,0,0,0,2,0,0,0,2,0,0,1},
                {1,0,0,0,0,0,0,0,0,2,0,0,1},
                {1,0,0,2,0,0,0,0,0,2,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,3,3,3,3,0,0,0,0,0,0,1},
                {1,0,3,0,0,3,0,0,0,0,0,0,1},
                {1,0,3,0,0,3,0,0,0,0,0,0,1},
                {1,0,3,0,0,0,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,1,1,1,1},
        };
        sizeBlock = 64;
    }

    public void stop(){
        if (statusSession != StatusSession.GAME_OVER){
            statusSession = StatusSession.PAUSE;
        }

        if (!running)
            return;

        running = false;

        try {
            gameThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void continueGame(){
        if (statusSession == StatusSession.GAME_OVER){
            return;
        }
        statusSession = StatusSession.ACTIVE;

        running = true;

        gameThread = new Thread((Runnable) this);
        gameThread.start();
    }

    public List<Enemy> getEnemies(){
        return enemies;
    }

    public int[][] getMap(){
        return map;
    }

    public int getSizeBlock(){
        return sizeBlock;
    }

    public Player getPlayer(){
        return player;
    }

    public StatusSession getStatus(){
        return statusSession;
    }

    public void setStatus(StatusSession status){
        if (statusSession != StatusSession.GAME_OVER){
            statusSession = status;
        }
    }

    public static double degreesToRadians(double degrees){
        return degrees * Math.PI / 180;
    }

    private boolean isCanMove(double x, double y){
        double sizeMap = map.length * sizeBlock;

        if ((x <= 0 || x >= sizeMap) || (y <= 0 || y >= sizeMap)){
            return false;
        }

        int i = (int) (y / sizeBlock) + ((y % sizeBlock != 0) ? 1 : 0) - 1;
        int j = (int) (x / sizeBlock) + ((x % sizeBlock != 0) ? 1 : 0) - 1;

        if (map[i][j] != 0){
            return false;
        }
        return true;
    }

    public synchronized void handleRequest(Command command){
        if ((statusSession == StatusSession.GAME_OVER) || (statusSession == StatusSession.WIN)){
            return;
        }
        switch (command){
            case ACTIVATE_BLOCK -> {
                double x = player.x;
                double y = player.y;
                int i = (int) (y / sizeBlock) + ((y % sizeBlock != 0) ? 1 : 0) - 1;
                int j = (int) (x / sizeBlock) + ((x % sizeBlock != 0) ? 1 : 0) - 1;


                if ((map[i][j + 1] == 4) || (map[i][j - 1] == 4) || (map[i + 1][j] == 4) || (map[i - 1][j] == 4)
                    || (map[i - 1][j - 1] == 4) || (map[i + 1][j - 1] == 4) || (map[i - 1][j + 1] == 4) || (map[i + 1][j + 1] == 4)){
                    statusSession = StatusSession.WIN;
                }

            }
            case PAUSE -> {
                statusSession = StatusSession.PAUSE;
            }
            case MOVE_DOWN -> {
//                System.out.println("move down");

                double angle = degreesToRadians(player.rotationAngle);

                double newX = player.x - Math.cos(angle) * MOVE;
                double newY = player.y - Math.sin(angle) * MOVE;

                if (!isCanMove(newX, newY)){
                    return;
                }

                player.x = newX;
                player.y = newY;
            }
            case MOVE_UP -> {
//                System.out.println("move up");

                double angle = degreesToRadians(player.rotationAngle);

                double newX = player.x + Math.cos(angle) * MOVE;
                double newY = player.y + Math.sin(angle) * MOVE;

                if (!isCanMove(newX, newY)){
                    return;
                }

                player.x = newX;
                player.y = newY;
            }
            case MOVE_LEFT -> {
//                System.out.println("move left");

                double angle = degreesToRadians(player.rotationAngle - 90);

                double newX = player.x + Math.cos(angle) * MOVE;
                double newY = player.y + Math.sin(angle) * MOVE;

                if (!isCanMove(newX, newY)){
                    return;
                }

                player.x = newX;
                player.y = newY;
            }
            case MOVE_RIGHT -> {
//                System.out.println("move right");

                double angle = degreesToRadians(player.rotationAngle - 90);

                double newX = player.x - Math.cos(angle) * MOVE;
                double newY = player.y - Math.sin(angle) * MOVE;

                if (!isCanMove(newX, newY)){
                    return;
                }

                player.x = newX;
                player.y = newY;
            }
            case ROTATE_LEFT -> {
//                System.out.println("rotate left");
                player.rotationAngle = (player.rotationAngle - ROTATION_ANGLE) % 360;
            }
            case ROTATE_RIGHT -> {
//                System.out.println("rotate right");
                player.rotationAngle = (player.rotationAngle + ROTATION_ANGLE) % 360;
            }
            case ATTACK -> {
//                System.out.println("fire");
                for (int i = 0; i < enemies.size(); ++i){
                    double px = player.x;
                    double py = player.y;

                    double ex = enemies.get(i).x;
                    double ey = enemies.get(i).y;

                    double distance = Point2D.Double.distance(px, py, ex, ey);
                    if (distance < 20){
                        enemies.get(i).decreaseHealth(player.getPower());
                        if (enemies.get(i).isDead()){
                            enemies.remove(i);
                        }
                    }
                }
            }
            default -> {}
        }
    }
}
