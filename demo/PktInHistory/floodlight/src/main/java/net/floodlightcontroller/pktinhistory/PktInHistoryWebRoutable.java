package net.floodlightcontroller.pktinhistory;

import net.floodlightcontroller.restserver.RestletRoutable;
import org.restlet.Context;
import org.restlet.Restlet;
import org.restlet.routing.Router;

public class PktInHistoryWebRoutable implements RestletRoutable {
    @Override
    public Restlet getRestlet(Context context) {
        Router router = new Router(context);
        router.attach("/history/json", PktInHistoryResource.class);
        return router;
    }

    @Override
    public String basePath(){
        return "/wm/pktinhistory";
    }
}
