#include "App.h"

App* App_Create(int width, int height, Raster*(*loadRaster)(const char*)) {
    App* app = (App*) malloc(sizeof(App));
    
    app->screen = Raster_Create(width, height);
    
    app->camera = Camera_Create(width, height, 75.0, 2.0, 10.0);
    app->camera->position.x = 1.5;
    app->camera->position.y = 1.5;
    
    app->tileRasters = (Raster**) malloc(256 * sizeof(Raster*));
    for (int i = 0; i < 256; i++) {
        app->tileRasters[i] = NULL;
    }
    
    app->tileRasters[0] = loadRaster("floor1.bmp");
    app->tileRasters[1] = loadRaster("floor2.bmp");
    app->tileRasters[2] = loadRaster("floor3.bmp");
    
    app->tileRasters[128] = loadRaster("wall1.bmp");
    app->tileRasters[129] = loadRaster("wall2.bmp");
    app->tileRasters[130] = loadRaster("wall3.bmp");
    
    app->map = createTestMap();
    
    app->leftPressed = 0;
    app->rightPressed = 0;
    app->downPressed = 0;
    app->upPressed = 0;
    app->strafeLeftPressed = 0;
    app->strafeRightPressed = 0;
    
    app->quit = 0;
    
    return app;
}

void App_Destroy(App* app) {
    if (app == NULL)
        return;
    
    Raster_Destroy(app->screen);
    Camera_Destroy(app->camera);
    for (int i = 0; i < 256; i++)
        Raster_Destroy(app->tileRasters[i]);
    free(app->tileRasters);
    Map_Destroy(app->map);
    
    free(app);
}

void App_HandleEvent(App* app, AppEventType type, AppEvent event) {
    if (type == APPEVENT_KEYPRESSED) {
        if (event.key == APPEVENT_KEY_ESCAPE) {
            app->quit = 1;
        }
        if (event.key == APPEVENT_KEY_LEFT) {
            app->leftPressed = 1;
        }
        if (event.key == APPEVENT_KEY_RIGHT) {
            app->rightPressed = 1;
        } 
        if (event.key == APPEVENT_KEY_DOWN) {
            app->downPressed = 1;
        } 
        if (event.key == APPEVENT_KEY_UP) {
            app->upPressed = 1;
        }
        if (event.key == APPEVENT_KEY_STRAFELEFT) {
            app->strafeLeftPressed = 1;
        }
        if (event.key == APPEVENT_KEY_STRAFERIGHT) {
            app->strafeRightPressed = 1;
        }
    } else if (type == APPEVENT_KEYRELEASED) {
        if (event.key == APPEVENT_KEY_LEFT) {
            app->leftPressed = 0;
        }
        if (event.key == APPEVENT_KEY_RIGHT) {
            app->rightPressed = 0;
        } 
        if (event.key == APPEVENT_KEY_DOWN) {
            app->downPressed = 0;
        } 
        if (event.key == APPEVENT_KEY_UP) {
            app->upPressed = 0;
        }
        if (event.key == APPEVENT_KEY_STRAFELEFT) {
            app->strafeLeftPressed = 0;
        }
        if (event.key == APPEVENT_KEY_STRAFERIGHT) {
            app->strafeRightPressed = 0;
        }
    }
}

void App_Render(App* app, double delta) {
    Camera* camera = app->camera;
    double move = 2.5 * (app->upPressed - app->downPressed);
    Vector moveAmt = {
        .x = move * sin(camera->rotation) * delta,
        .y = move * cos(camera->rotation) * delta
    };
    double strafe = 1.8 * (app->strafeLeftPressed - app->strafeRightPressed);
    Vector strafeAmt = {
        .x = strafe * sin(camera->rotation + 1.571) * delta,
        .y = strafe * cos(camera->rotation + 1.571) * delta
    };
    double turn = 3.0 * (app->leftPressed - app->rightPressed);
    double turnAmt = turn * delta;
    camera->position.x += moveAmt.x;
    camera->position.y += moveAmt.y;
    camera->position.x += strafeAmt.x;
    camera->position.y += strafeAmt.y;
    camera->rotation += turnAmt;
    
    renderFloors(app->screen, app->map, camera, app->tileRasters);
    renderWalls(app->screen, app->map, camera, app->tileRasters);
}

int App_Continue(App* app) {
    return !app->quit;
}