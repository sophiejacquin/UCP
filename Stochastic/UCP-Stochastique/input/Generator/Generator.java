
import java.io.*;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Henri LARGET
 */
public class Generator {

    public static final Double version = 1.0;
    private final Random o = new Random();
    private List<Unit> units;
    private List<Double> loads;
    private String invariant_begin = "";
    private String invariant_end = "";
    private String invariant_middle = "";
    private String filePath;
    private int scenario;
    private BufferedReader bufRead;
    private int param_variation_on_loads = 20;
    private int param_variation_on_unit = 0;

    /**
     * @param array args the command line arguments
     */
    public static void main(String[] args) {

        Generator generator;
        int sce = 20, dl = 20, du = 0;
        String filename = "";
        int ct = 0;
        if (args.length < 1) {
        }
        for (int i = 0; i < args.length; i++) {
            if (args[i].startsWith("-v")) {
                System.out.println("Scenario Generator v" + Generator.version);
                System.exit(0);
            } else if (args[i].startsWith("-h")) {
                how_it_works();
                System.exit(0);
            } else if (args[i].startsWith("-n")) {
                sce = Integer.parseInt(args[i + 1]);
                i++;
            } else if (args[i].startsWith("-dl")) {
                dl = Integer.parseInt(args[i + 1]);
                i++;
            } else if (args[i].startsWith("-du")) {
                du = Integer.parseInt(args[i + 1]);
                i++;
            } else if (args[i].startsWith("-")) {
                Generator.how_it_works();
                System.exit(1);
            } else {
                ct++;
                filename = args[i];
                if (ct > 1) {
                    Generator.how_it_works();
                    System.exit(1);
                }
            }

        }
        if (dl == 0) {
            dl = 20;
        }
        if (args.length < 1 || filename.equals("")) {
            Generator.how_it_works();
            System.exit(1);
        }
        generator = new Generator(filename, sce, dl, du);
        System.out.println(filename + " : " + sce + " Scenarios by type\n" + dl + "% on viariation for loads\n" + du + "% on variation for units");
        generator.generate();

    }

    /**
     * prints how the command works.
     */
    public static void how_it_works() {

        System.err.println("java generator -h");
        System.err.println("the syntax is :");
        System.err.println("*   java generator /path/to/file.tsp [-n number_of_scenarios] [-dl variation_on_load] [-du variation_on_utils]");
        System.err.println("*        -n number_of_scenarios  : number of scenarios to generate by types. 20 by default. ");
        System.err.println("*        -dl variation_on_load   : Variation (Percent) on loads : 20% by default ");
        System.err.println("*        -du variation_on_units  : Variation (Percent) on Units : Standard deviation between pmin and pmax by default");

    }

    public Generator(String filePath, int scenario, int dl, int du) {
        this.filePath = filePath;
        this.scenario = scenario;
        this.param_variation_on_loads = dl;
        this.param_variation_on_unit = du;

    }

    public Generator(String filePath, int scenario, int dl) {
        this.filePath = filePath;
        this.scenario = scenario;
        this.param_variation_on_loads = dl;

    }

    public Generator(String filePath, int scenario) {
        this.filePath = filePath;
        this.scenario = scenario;

    }

    public Generator(String filePath) {
        this.filePath = filePath;
        this.scenario = 20;
    }

    /**
     * main method of the function. creates the folder, reads the file and generates the outputs.
     */
    public void generate() {
        units = new ArrayList<Unit>();
        loads = new ArrayList<Double>();
        if (this.load_file(this.filePath)) {
            System.out.println("OK : " + this.filePath);
            if (this.init_folder(this.filePath)) {
                this.read_file();
                this.output(true, false);
                this.output(false, true);
                this.output(true, true);


            } else {
                System.err.println("unable to create folder or folder already exists. abort.");
            }
        } else {
            System.err.println("failed");
        }
    }

    /**
     * Load the base file
     * @param String the file to read
     * @return TRUE if the file is loaded
     */
    public boolean load_file(String filePath) {
        try {
            FileReader input;
            input = new FileReader(filePath);
            this.bufRead = new BufferedReader(input);
        } catch (FileNotFoundException ex) {
            System.err.println("file doesn't exist : " + filePath);
            return false;
        }
        return true;
    }

    /**
     * Read and save into variables the file data
     */
    public void read_file() {
        String row;
        Boolean begin = true;
        Boolean middle = false;
        Unit u;
        try {
            while ((row = this.bufRead.readLine()) != null) {
                row = row.trim();
                if (row.startsWith("@")) {
                    begin = false;
                    middle = true;
                    row = row.replaceAll(" +", "");
                    String spliter[] = row.split(",");
                    int unit_id = Integer.parseInt(spliter[0].replace("@", ""));
                    if (spliter.length == 12) {
                        u = new Unit(
                                unit_id,
                                Double.parseDouble(spliter[1]),
                                Double.parseDouble(spliter[2]),
                                Double.parseDouble(spliter[3]),
                                Double.parseDouble(spliter[4]),
                                Double.parseDouble(spliter[5]),
                                Double.parseDouble(spliter[6]),
                                Double.parseDouble(spliter[7]),
                                Double.parseDouble(spliter[8]),
                                Double.parseDouble(spliter[9]),
                                Double.parseDouble(spliter[10]),
                                Double.parseDouble(spliter[11]));
                        units.add(u);
                    } else {
                        System.err.println("Invalid unit format. expecting @int, double, double, double, double, double, double, double, double, double, double, double");
                    }
                } else if (row.startsWith("%l")) {
                    middle = false;
                    String[] elements = row.replace("%l", "  ").trim().split(",");
                    for (int i = 0; i < elements.length; i++) {
                        loads.add(Double.parseDouble(elements[i]));
                    }
                } else {
                    if (begin && !middle) {
                        invariant_begin += row + "\n";
                    } else if (!begin && middle) {
                        invariant_middle += row + "\n";
                    } else {
                        invariant_end += row + "\n";
                    }
                }
            }

        } catch (IOException ex) {
            System.err.println("enable to read file.");
        }
    }

    /**
     * creates the output folder, the output files and write inside
     * @param unit_modifications  TRUE if you want to make variations on the units
     * @param loads_modifications TRUE if you want to make variations on loads
     */
    public void output(Boolean unit_modifications, Boolean loads_modifications) {
        DecimalFormat formatter = new DecimalFormat("##0");

        String file_name;
        try {
           /*
			if (unit_modifications && loads_modifications) {
                file_name = "mixed";
            } else if (loads_modifications) {
                file_name = "loads";
            } else {
                file_name = "units";
                unit_modifications = true;
            }
			*/

            File f = new File("." + File.separator + "scenarios");//+File.separator+file_name);
            if (f.exists()) {
                System.out.println("folder selected : " + f.getCanonicalPath());
                PrintWriter writer;
				int i = 1+this.scenario*((loads_modifications?1:0)+(loads_modifications&&unit_modifications?1:0));
				int imax = i+this.scenario;
                for (; i < imax; i++) {
                    writer = new PrintWriter(f.getCanonicalPath() + File.separator + "scenario_" + i + ".tsp", "UTF-8");
                    writer.println(invariant_begin);
                    for (Unit ut : units) {
                        if (unit_modifications) {
                            ut.transform(this.param_variation_on_unit);
                        } else {
                            ut.reset();
                        }
                        writer.println(ut);
                    }
                    writer.println(invariant_middle);
                    writer.print("%l   ");
                    if (loads_modifications) {
                        for (Double lo : loads) {
                            writer.print(formatter.format(this.transform_loads(lo)).replace(",", "."));
                            writer.print(",");
                        }
                    } else {
                        for (Double lo : loads) {
                            writer.print(formatter.format(lo).replace(",", "."));
                            writer.print(",");
                        }
                    }
                    writer.print("\n");

                    writer.println(invariant_end);
                    writer.close();
                }
            }
        } catch (IOException ex) {
            System.err.println("unable to write files.");
        }
    }

    /**
     *
     * @param load the value to transform
     * @return the transformed value of load by variation between 0 & 20%
     */
    public Double transform_loads(Double load) {
        load = load - (o.nextGaussian() / 4) * (load * this.param_variation_on_loads / 100);

        return load;
    }

    /**
     * create the initial folder and copy the original problem inside
     * @return success on creation
     */
    public Boolean init_folder(String initial_file) {
        try {
            File f = new File("." + File.separator + "scenarios");
            FileReader input;
            input = new FileReader(initial_file);
            BufferedReader br = new BufferedReader(input);
            boolean c = f.mkdir();
            if (c) {
                try {
                    String row;
                    PrintWriter wr = new PrintWriter(f.getCanonicalPath() + File.separator + "scenario_0.tsp", "UTF-8");
                    while ((row = br.readLine()) != null) {
                        wr.println(row);
                    }
                    wr.close();
                } catch (IOException ex) {
                    System.err.println("unable to copy the file in the new folder");
                }

            }
            return c;
        } catch (FileNotFoundException ex) {
            return false;
        }
    }
}
