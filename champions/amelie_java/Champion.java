import java.util.ArrayList;
import java.util.List;

public class Champion extends Api
{
    protected int my_id;
    protected int adv_id;

    /**
     * Fonction appelée au début de la partie.
     */
    public void partie_init()
    {
        my_id = moi();
        adv_id = adversaire();
    }

    private Position new_pos(int x, int y)
    {
        Position p = new Position();
        p.x = x;
        p.y = y;
        return p;
    }

    private int distance(Position p1, Position p2)
    {
        return Math.abs(p1.x - p2.x) + Math.abs(p1.y - p2.y);
    }

    private List<Plante> find_plants_around(Plante p, int radius)
    {
        List<Plante> adv_plants = new ArrayList<Plante>();
        if (radius < 0)
            radius = TAILLE_GRILLE / 4;

        for (int i = - radius; i < radius + 1; ++i)
        {
            int pos_x = Math.min(Math.max(i + p.pos.x, 0), TAILLE_GRILLE - 1);

            int radius2 = radius - Math.abs(i);
            for (int j = Math.max(p.pos.y - radius2, 0); j < Math.min(p.pos.y + radius2 + 1, TAILLE_GRILLE); ++j)
            {
                Position pos = new_pos(pos_x, j);

                Plante tmp = plante_sur_case(pos);
                if (tmp.jardinier == adv_id)
                    adv_plants.add(tmp);
            }
        }

        return adv_plants;
    }

    private void arroser_my_plants(int round)
    {
        Plante[] my_plants = plantes_arrosables(my_id);

        if (round < 10)
        {
            for (Plante p : my_plants)
            {
                Erreur e = arroser(p.pos, Caracteristique.CARACTERISTIQUE_FORCE);
                if (e == Erreur.OK)
                    continue;

                debug_afficher_chien(p.pos, DebugChien.CHIEN_BLEU);
            }
        }
        else
        {
            List<Tuple<Integer, Plante>> nb_arounds = new ArrayList<Tuple<Integer, Plante>>();
            for (Plante p : my_plants)
                nb_arounds.add(new Tuple<Integer, Plante>(find_plants_around(p, -1).size(), p));

            nb_arounds.sort((Tuple<Integer, Plante> t1, Tuple<Integer, Plante> t2) -> t1.x < t2.x ? -1 : t1.x == t2.x ? 0 : 1);

            int mid_value = nb_arounds.get(nb_arounds.size() / 2).x;
            
            int end_one = 0;
            while (end_one < nb_arounds.size() && nb_arounds.get(end_one).x > 0)
                end_one += 1;

            mid_value = mid_value > 0 ? mid_value : end_one;

            // proche des adversaires
            for (int i = 0; i < mid_value; ++i)
            {
                Plante p = nb_arounds.get(i).y;
                Erreur e = arroser(p.pos, Caracteristique.CARACTERISTIQUE_FORCE);
                if (e == Erreur.OK)
                    continue;

                debug_afficher_chien(p.pos, DebugChien.CHIEN_BLEU);
            }

            // des adversaires pas loin
            for (int i = mid_value; i < end_one; ++i)
            {
                Plante p = nb_arounds.get(i).y;
                Erreur e = arroser(p.pos, Caracteristique.CARACTERISTIQUE_VIE);
                if (e == Erreur.OK)
                    continue;

                debug_afficher_chien(p.pos, DebugChien.CHIEN_BLEU);
            }

            // zone safe
            for (int i = end_one; i < nb_arounds.size(); ++i)
            {
                Plante p = nb_arounds.get(i).y;
                Erreur e = arroser(p.pos, Caracteristique.CARACTERISTIQUE_ELEGANCE);
                if (e == Erreur.OK)
                    continue;

                debug_afficher_chien(p.pos, DebugChien.CHIEN_BLEU);
            }
        }
    }

    private int[] sum_array(int[] array1, int[] array2)
    {
        if (array1.length != array2.length)
            return null;

        for (int i = 0; i < array1.length; ++i)
            array1[i] += array2[i];

        return array1;
    }

    private int[] remove_cost(int[] array, int cost)
    {
        for (int i = 0; i < array.length; ++i)
            array[i] -= cost;

        return array;
    }

    private int[] compute_ressource_collecte(Position p, int rayon_collecte)
    {
        int[] ressource_collecte = {0, 0, 0};

        for (int i = - rayon_collecte; i < rayon_collecte + 1; ++i)
        {
            int pos_x = Math.min(Math.max(i + p.x, 0), TAILLE_GRILLE - 1);

            int radius2 = rayon_collecte - Math.abs(i);
            for (int j = Math.max(p.y - radius2, 0); j < Math.min(p.y + radius2 + 1, TAILLE_GRILLE); ++j)
            {
                Position p2 = new_pos(pos_x, j);

                int[] ressources = ressources_sur_case(p2);

                int[] remaining_ressources = remove_cost(ressources, distance(p, p2) * COUT_PAR_CASE_COLLECTE);

                ressource_collecte = sum_array(ressource_collecte, remaining_ressources);
            }
        }

        return ressource_collecte;
    }

    private int compare_ressources(int[] res1, int[] res2)
    {
        int diff = 0;
        for (int i = 0; i < 3; ++i)
            diff += Math.abs(res1[i] - res2[i]);

        if (diff == 0)
            return 0;
        if (diff < 0)
            return -1;
        return 1;
    }

    private void depoter_my_plants(int round)
    {
        if (round % 2 == 0)
            return;

        Plante[] plantes = plantes_depotables(my_id);

        for (int i = 0; i < plantes.length; ++i)
        {
            if (i % 3 != 0)
                continue;

            ArrayList<Tuple<Position, int[]>> positions = new ArrayList<>();

            for (int i2 = 0; i2 < TAILLE_GRILLE; ++i2)
                for (int j = 0; j < TAILLE_GRILLE; ++j)
                {
                    Position p2 = new_pos(i, j);

                    int[] ressource_collecte_possible = compute_ressource_collecte(p2, plantes[i].rayon_collecte);

                    if (distance(plantes[i].pos, p2) <= plantes[i].rayon_deplacement)
                        positions.add(new Tuple<Position, int[]>(p2, ressource_collecte_possible));
                }

            positions.sort((Tuple<Position, int[]> t1, Tuple<Position, int[]> t2) -> compare_ressources(t1.y, t2.y));

            Erreur e = depoter(plantes[i].pos, positions.get(0).x);
            if (e == Erreur.OK)
                continue;

            debug_afficher_chien(plantes[i].pos, DebugChien.CHIEN_VERT);
        }
    }

    private void baffer_plants()
    {
        Plante[] my_plants = plantes_jardinier(my_id);
        for (Plante p : my_plants)
        {
            List<Plante> plantes_around = find_plants_around(p, PORTEE_BAFFE);
            // trie la liste par point de vie
            plantes_around.sort((Plante p1, Plante p2) -> p1.vie < p2.vie ? -1 : p1.vie == p2.vie ? 0 : 1 );
            if (plantes_around.isEmpty())
                continue;

            Erreur e = baffer(p.pos, plantes_around.get(0).pos);
            if (e == Erreur.OK)
                continue;

            debug_afficher_chien(p.pos, DebugChien.CHIEN_ROUGE);
        }
    }

    private void call_hist_functions()
    {
        ActionHist[] hist = historique();
        int depoter = 0;
        int baffer = 0;
        int arroser = 0;

        for (ActionHist action: hist)
        {
            if (action.atype == ActionType.ACTION_DEPOTER)
                depoter += 1;
            else if (action.atype == ActionType.ACTION_BAFFER)
                baffer += 1;
            else
                arroser += 1;
        }
    }

    /**
     * Fonction appelée à chaque tour.
     */
    public void jouer_tour()
    {
        int round = tour_actuel();
        
        arroser_my_plants(round);

        depoter_my_plants(round);

        baffer_plants();

        call_hist_functions();
    }

    /**
     * Fonction appelée à la fin de la partie.
     */
    public void partie_fin()
    {
        // TODO
    }

    public class Tuple<X, Y> { 
        public final X x; 
        public final Y y; 
        public Tuple(X x, Y y) { 
          this.x = x; 
          this.y = y; 
        } 
    }
}
