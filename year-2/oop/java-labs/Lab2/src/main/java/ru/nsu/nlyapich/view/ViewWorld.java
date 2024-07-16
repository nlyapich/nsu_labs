package ru.nsu.nlyapich.view;

import ru.nsu.nlyapich.Display;
import ru.nsu.nlyapich.model.ModelWorld;

import java.awt.*;
import java.awt.geom.Rectangle2D;

public class ViewWorld implements ViewerModel {
    private ModelWorld world;
    private final int HEIGHT;
    private final int WIDTH;
    public ViewWorld(ModelWorld world, int HEIGHT, int WIDTH){
        this.world = world;
        this.HEIGHT = HEIGHT;
        this.WIDTH = WIDTH;
    }
    private void renderGround(Graphics2D graphics){
        Rectangle2D.Double ground = new Rectangle2D.Double(0, HEIGHT/2, WIDTH, HEIGHT/2);
        graphics.setColor(Color.YELLOW);

        graphics.fill(ground);
    }

    private void renderSky(Graphics2D graphics){
        Rectangle2D.Double sky = new Rectangle2D.Double(0, 0, WIDTH, HEIGHT/2);
        graphics.setColor(Color.BLUE);

        graphics.fill(sky);
    }

    private void renderCamera(Graphics2D graphics){
        Camera.rayCasting(world, Display.getGraphics(), HEIGHT, WIDTH);
    }

    private void renderPlayerHealth(Graphics2D graphics){
        int maxHealth = world.getPlayer().getMaxHealth();
        int curHealth = world.getPlayer().getCurHealth();

        int height = 10;
        int x = 10;
        int y = 10;
        graphics.setColor(Color.BLACK);
        graphics.fillRect(x, y, maxHealth, height);
        graphics.setColor(Color.RED);
        graphics.fillRect(x, y, curHealth, height);
    }

    @Override
    public void render(Graphics2D graphics){
        renderSky(graphics);
        renderGround(graphics);
        renderCamera(graphics);
        renderPlayerHealth(graphics);
    }
}
