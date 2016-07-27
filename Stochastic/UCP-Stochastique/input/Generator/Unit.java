
import java.text.DecimalFormat;
import java.util.Random;

public class Unit {

    private final Random o = new Random();
    private int unit_id = 1;
    private double pmax_transform, pmin_transform, pmax, pmin, a, b, c, min_up, min_dn, hot_start_cost, cold_start_cost, cold_start_hrs, initial_status;

    public void transform(int percent) {
        pmin_transform = pmin;

        if (percent == 0) {
            pmax_transform = pmax - (Math.abs(o.nextGaussian()) / 4) * (pmax - pmin);
        } else {
            percent = percent / 100;
            pmax_transform = pmax - (pmax * (Math.abs(o.nextGaussian()) / 4) * (percent));
            pmax_transform = pmax_transform < 0 ? 0 : pmax_transform;

            pmin_transform = pmin - (pmax - pmax_transform) > 0 ? pmin - (pmax - pmax_transform) : 0;

        }
    }

    public void reset() {
        pmax_transform = pmax;
        pmin_transform = pmin;

    }

    public Unit(int unit_id, double pmax, double pmin, double a, double b, double c, double min_up, double min_dn, double hot_start_cost, double cold_start_cost, double cold_start_hrs, double initial_status) {
        this.unit_id = unit_id;
        this.pmax = pmax;
        this.pmin = pmin;
        this.a = a;
        this.b = b;
        this.c = c;
        this.min_up = min_up;
        this.min_dn = min_dn;
        this.hot_start_cost = hot_start_cost;
        this.cold_start_cost = cold_start_cost;
        this.cold_start_hrs = cold_start_hrs;
        this.initial_status = initial_status;
        this.pmax_transform = pmax;
        this.pmin_transform = pmin;
    }

    public String toString() {
        DecimalFormat formatter = new DecimalFormat("##0.####");
        DecimalFormat formatter2 = new DecimalFormat("##0.");

        String out = "@" + unit_id + ", ";
        out += ((int) pmax_transform) + ", ";
        out += ((int) pmin_transform) + ", ";
        out += formatter.format(a).replace(",", ".") + ", ";
        out += formatter.format(b).replace(",", ".") + ", ";
        out += formatter.format(c).replace(",", ".") + ", ";
        out += formatter.format(min_up).replace(",", ".") + ", ";
        out += formatter.format(min_dn).replace(",", ".") + ", ";
        out += formatter.format(hot_start_cost).replace(",", ".") + ", ";
        out += formatter.format(cold_start_cost).replace(",", ".") + ", ";
        out += formatter.format(cold_start_hrs).replace(",", ".") + ", ";
        out += formatter.format(initial_status).replace(",", ".");
        return out;
    }
}
