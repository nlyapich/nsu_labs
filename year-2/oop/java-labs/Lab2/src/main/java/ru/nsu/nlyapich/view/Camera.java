package ru.nsu.nlyapich.view;

import ru.nsu.nlyapich.Line;
import ru.nsu.nlyapich.model.Enemy;
import ru.nsu.nlyapich.model.ModelWorld;
import ru.nsu.nlyapich.model.Player;

import java.awt.*;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.util.List;

import static java.lang.Math.*;

public class Camera {
    //Viewing range
    private static final double viewRange = 1000;
    //Angle viewing in degrees
    private static final double viewAngle = 75;
    //Distance between Ray
    private static final double distanceRays = 0.05;
    public Camera(){}

    public static double degreesToRadians(double degrees){
        return degrees * Math.PI / 180;
    }

    private static Point2D.Double getLineCoefficients(Line line, double k, double b) {
        if (line.p1.x == line.p2.x) {
            return null;
        }
        k = (line.p2.y - line.p1.y) / (line.p2.x - line.p1.x);

        b = line.p1.y - k * line.p1.x;

        return new Point2D.Double(k, b);
    }

    private static boolean BelongingInterval(double min, double max, double num, double error){
        if(min > max){
            double tmp = max;
            max = min;
            min = tmp;
        }

        if(num >= min - error && num <= max + error){
            return true;
        } else{
            return false;
        }
    }

    private static Point2D.Double crossingLines(Line line1, Line line2)
    {
        Point2D.Double pCrossing = new Point2D.Double();
        double error = 0.5;

        boolean isParallelY_1 = true;
        double k1 = Double.MAX_VALUE, b1 = Double.MAX_VALUE;
        if(abs(line1.p1.x - line1.p2.x) >= error)
        {
            Point2D.Double tmp = getLineCoefficients(line1, k1, b1);
            if (tmp != null){
                k1 = tmp.x;
                b1 = tmp.y;
            }
            isParallelY_1 = false;
        }

        boolean isParallelY_2 = true;
        double k2 = Double.MAX_VALUE, b2 = Double.MAX_VALUE;
        if(abs(line2.p1.x - line2.p2.x) >= error) {
            Point2D.Double tmp = getLineCoefficients(line2, k2, b2);
            if (tmp != null){
                k2 = tmp.x;
                b2 = tmp.y;
            }
            isParallelY_2 = false;
        }

        if(k1 == k2)
        {
            return null;
        }

        if (isParallelY_1)
        {
            pCrossing.x = line1.p1.x;
            pCrossing.y = k2 * (pCrossing.x) + b2;
        }
        else if (isParallelY_2)
        {
            pCrossing.x = line2.p1.x;
            pCrossing.y = k1 * (pCrossing.x) + b1;
        }
        else
        {
            pCrossing.x = (b2 - b1) / (k1 - k2);
            pCrossing.y = k1 * (pCrossing.x) + b1;
        }

        if(!BelongingInterval(line1.p1.x, line1.p2.x, pCrossing.x, error) ||
                !BelongingInterval(line1.p1.y, line1.p2.y, pCrossing.y, error) ||
                !BelongingInterval(line2.p1.x, line2.p2.x, pCrossing.x, error) ||
                !BelongingInterval(line2.p1.y, line2.p2.y, pCrossing.y, error))
        {
            return null;
        }

        return pCrossing;
    }

    private static Color getColor(int block){
        switch (block){
            case 1 -> {
                return Color.GREEN;
            }
            case 2 -> {
                return Color.PINK;
            }
            case 3 -> {
                return Color.RED;
            }
            case 4 -> {
                return Color.MAGENTA;
            }
            default -> {
                return Color.BLUE;
            }
        }
    }

    public static void rayCasting(ModelWorld world, Graphics2D g, int wHeight, int wWidth){
        int[][] map = world.getMap();
        Player player = world.getPlayer();
        List<Enemy> enemies = world.getEnemies();
        int sizeBlock = world.getSizeBlock();
        int sizeMap = map.length;

        double angleNow = -(viewAngle / 2);
        for(int i = 1; angleNow <= viewAngle / 2; i++)
        {
            double angleRadians = degreesToRadians(player.rotationAngle + angleNow);

            Point2D.Double pos =  new Point2D.Double(0,0);
            pos.x = player.x + Math.cos(angleRadians) * viewRange;
            pos.y = player.y + Math.sin(angleRadians) * viewRange;

            Point obj   = new Point();

            double minDistance = Double.MAX_VALUE;

            for(int j = 0; j < sizeMap; j++)
            {
                for (int k = 0; k < sizeMap; k++) {

                    if (map[j][k] == 0){
                        continue;
                    }

                    Point2D.Double[] blockPoints = new Point2D.Double[] {
                        new Point2D.Double(k * sizeBlock, j * sizeBlock),
                        new Point2D.Double((k + 1) * sizeBlock, j * sizeBlock),
                        new Point2D.Double((k + 1) * sizeBlock, (j + 1) * sizeBlock),
                        new Point2D.Double(k * sizeBlock, (j + 1) * sizeBlock),
                    };

                    for (int p = 0; p < blockPoints.length; p++) {
                        Line tmp1 = new Line(player.x, player.y, pos.x, pos.y);
                        Line tmp2 = new Line(
                                blockPoints[p],
                                blockPoints[(p + 1) % blockPoints.length]
                        );

                        Point2D.Double tmpPointCrossing = crossingLines(tmp1, tmp2);
                        if (tmpPointCrossing != null) {
                            double distance = tmpPointCrossing.distance(player.x, player.y);

                            distance *= cos(degreesToRadians(angleNow));

                            if (distance < minDistance) {
                                minDistance = distance;

                                obj.x = j;
                                obj.y = k;
                            }
                        }
                    }
                }
            }

            int numEnemy = -1;
            double minDistanceEnemy = minDistance;
            for (int m = 0; m < enemies.size(); ++m){
                double ex = enemies.get(m).x;
                double ey = enemies.get(m).y;
                Line tmp1 = new Line(player.x, player.y, pos.x, pos.y);
                Line tmp2 = new Line(
                        ex - 2, ey - 2,
                        ex + 2, ey + 2
                );
                Point2D.Double tmpPointCrossing = crossingLines(tmp1, tmp2);
                if (tmpPointCrossing != null) {
                    double distance = tmpPointCrossing.distance(player.x, player.y);

                    distance *= cos(degreesToRadians(angleNow));

                    if (distance < minDistanceEnemy) {
                        minDistanceEnemy = distance;

                        numEnemy = m;
                    }
                }
            }

            if(minDistance != Double.MAX_VALUE)
            {
                Rectangle2D.Double rectangle = new Rectangle2D.Double();

                double width  = wWidth / (viewAngle / distanceRays);
                double height = sizeBlock * wHeight  / minDistance;

                rectangle.width = width;
                rectangle.height = height;
                rectangle.x = (i - 1) * width;
                rectangle.y = (wHeight - height) / 2;

                g.setColor(getColor(map[obj.x][obj.y]));
                g.draw(rectangle);

                if (numEnemy != -1) {
                    double newHeight = 4 * wHeight / minDistanceEnemy;
                    rectangle.height = newHeight;
                    rectangle.y = (wHeight - height) / 2 + (height / 2);
                    g.setColor(Color.BLACK);
                    g.draw(rectangle);
                }
            }

            angleNow += distanceRays;
        }
    }
}
