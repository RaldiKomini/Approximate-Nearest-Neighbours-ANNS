# Ανάπτυξη Λογισμικού για Αλγοριθμικά Προβλήματα <br/> Εργασια 1 <br/><br/> ΦΟΙΤΗΤΕΣ:
    Ονοματεπώνυμο: ΡΑΛΝΤΙΟΝ ΚΟΜΙΝΙ  
    Ονοματεπώνυμο: ΑΝΤΡΕΪ-ΑΝΤΡΙΑΝ ΠΡΕΝΤΑ    
    

<br/>


## ΦΑΚΕΛΟΙ

+ programs: Εκτελέσιμα προγράμματα. lsh.cpp, hypercube.cpp, cluster.cpp
          
+ modules: Περιεχει ολα τα modules που χρησιμοποιουν τα αρχεια στο Programs για να τρεξουν

        pr_modl.cpp:  Εχει βασικες συναρτησεις που χρειαζονται στα υπολοιπα αρχεια(υπολογισμος αποστασεων, read,
                      εσωτερικο γινομενο κλπ)

        methods_modl.cpp:  Συναρτήσεις της Method. Ειναι μια γενικη δομη που χρησιμοποιουν το lsh και το hypercube

        lsh_modl.cpp:    Συναρτησεις για το lsh
        hypercube_modl.cpp:  Συναρτησεις για το hypercube
        cluster_modl.cpp:    Συναρτησεις για το cluster

+ include: Περιεχει τις επικεφαλιδες ολων των αρχειων παραπανω


<br>

##  ΟΔΗΓΙΕΣ ΜΕΤΑΓΛΩΤΤΙΣΗΣ

    $ make lsh
    $ make cube
    $ make cluster

    $ make lsh_clean
    $ make cube_clean
    $ make cluster_clean

Για διαγραφή του γενικού εκτελέσιμου:
    
    $ make clean



### Χρήση εκτελέσιμου:

    ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>
    , δηλαδη
    ./lsh -d train-images.idx3-ubyte -q t10k-images.idx3-ubyte -k 4 -L 5 -o lsh_out.txt -N 10 -R 1000
    
    ./cube –d <input file> –q <query file> –k <int> -M <int> --probes <int> -ο <output file> -Ν <number of nearest> -R <radius>
    , δηλαδη
    ./cube -d train-images.idx3-ubyte -q t10k-images.idx3-ubyte -k 9 -M 1000 --probes 6 -o cube_out.txt -N 10 -R 2000

    ./cluster –i <input file> –c <configuration file> -o <output file> --complete <optional> -m <method: Classic OR LSH or Hypercube>
    , δηλαδη
    /cluster -i train-images.idx3-ubyte -c cluster.conf -o cluster_out.txt --complete -m Classic

<br>

##  ΚΛΑΣΕΙΣ

Υλοποιηση κλασης Method:
Εχει protected μεταβλητες για να μπορουν να τις χρησιμοποιουν οι LSH και Cube που ειναι παιδια της.
Οι συναρτησεις vector<point> nearest_search(int N) και vector<int> range_search(double R) ειναι public ωστε να μπορει να τις καλει κατευθειαν ο χρηστης. Δεν υπαρχουν αντιστοιχες στις κλασεις LSH και Cube, χρησιμοποιουμε αυτες.

    Public:
        Η nearest_search() επιστρεφει vector απο points, οπου το point ειναι δομη {double dist; int id;} και κραταει την αποσταση και το id της εικονας.
        Το καναμε να επιστρεφει αυτο γιατι στην εκφωνηση στην nearest_search θελει να εκτυπωνουμε και θεση εικονας και αποσταση.

        Στο range_search() ζηταει μονο αριθμο εικονας οποτε επιστρεφουμε vector απο int  που κραταει τους αριθμους τον εικονων.

    Private:
        Η hfunc_factors() δημιουργει τα t και v που χρειαζομαστε για τις συναρτησεις h και τα αποθηκευει στο αντικειμενο της κλασης
        Η calc_h() υπολογιζει το h(p) συμφωνα με τις διαφανειες 
        
Η brute_nearest() βρισκει με brute force τους Ν πλησιέστερους γειτονες μιας εικονας




### Υλοποιηση κλασης LSH:
    Η κλαση LSH ειναι παιδι της Method

    Private: 
        calc_g(): Υπολογιζει και επιστρεφει την τιμη g της εικονας συμφωνα με την θεωρια

    Public: 
        query(q): Βρισκει σε ποια buckets των hash tables ανηκει η εικονα q και κραταει τις εικονες που βρισκονται στο ιδιο bucket
        Ο χρηστης πρεπει να την τρεχει πριν καλεσει nearest_search() ή range_search()





### Υλοποιηση κλασης Cube:
    Η κλαση Cube ειναι και αυτη παιδι της Method
    
    Private: 
        calc_fh(): Υπολογιζει την δυαδικη αναπαραστηαση της εικονας και επιστρεφει την τιμη της σε δεκαδικη αναπαρασταση
                   Δηλαδη αν η εικονα ειναι {1,0,0,1} θα επιστρεψει 9 και το 9 θα το χρησιμοποιησουμε ως key για το map

        myperm(): Χρησιμοποιειται για να βρεθουν οι γειτονικες ακμες με hamming distance 1 ή 2

        bin_to_dec(): Μετατρεπει εναν πινανα απο 0,1 σε δεκαδικο αριθμο, χρησιμοποιειται στην calc_fh()

    Public: 
        query(q): Βρισκει τους M κοντινοτεορυς γειτονες και τους αποθηκευει στο nNearest
        Ο χρηστης πρεπει να την τρεχει πριν καλεσει nearest_search() ή range_search()


### Υλοποιηση cluster:

Συναρτησεις:
    
    cK_check(): Ελεγχει αν τα clusters δεν εχουν μετακινηθει πολυ σε σχεση με την προηγουμενη επαναληψη σταματαει την λουπα

    bin_search(): Δυαδικη αναζητηση που χρησιμοποιουμε στον k-means++ για να υπολογισουμε σε ποια εικονα αντιστοιχει ο τυχαιος αριθμος για την επιλογη εικονας

    Silhouette(): Υπολογιζει τις σιλουετες που ζητουνται

    kmeans(): Αρχικοποιει τα centroids με βαση τον αλγοριθμο k-means++

Δεν φτιαξαμε κλαση για το cluster γιατι απλα χρησιμοποιει τις προηγουμενες. Δεν αποθηκευει πολλα δεδομενα, δεν χρειαζοταν και δεν βολευε.


<br>

## ΑΛΛΑ ΑΡΧΕΙΑ 

+ pr.h:   Εχει βασικες συναρτησεις που χρησιμοποιονυται παντου

Συναρτησεις:

    p_norm(): Για τον υπολογισμο της αποστασης με νορμα p

    dot_prod():Για τον υπολογισμο εσωτερικου γινομενου

    meta_read();    image_read();   get_labels(); : Για τα δεοδμενα του mnist

    


Για LSH/ Hypercube και Cluster υπαρχει αντιστοιχο option struct, ωστε να αποθηκευονται τα ορισματα και να χρησιμοποιηθουν κατα την εκτελεση των προγραμματων. Τα 3 αυτα struct εχουν επισης μεταβλητη τυπου File, οπου σε αυτην αποθηκευονται τα αρχεια εισοδου, query/config και εξοδου, και μεταβλητη τυπου ofstream, ωστε να γραφτουν τα αποτελεσματα στο αρχειο εξοδου.




<br>
 
 ---

<br><br>

# Ανάπτυξη Λογισμικού για Αλγοριθμικά Προβλήματα <br/> Εργασια 2 <br/><br/> ΦΟΙΤΗΤΕΣ:
    Ονοματεπώνυμο: ΡΑΛΝΤΙΟΝ ΚΟΜΙΝΙ
    Ονοματεπώνυμο: ΑΝΤΡΕΪ-ΑΝΤΡΙΑΝ ΠΡΕΝΤΑ
    


<br/>

## Περιγραφή

#### Στην εργασία αυτή έχουν υλοποιηθεί τα δύο διαφορετικά ευρετήρια γράφων, ο k-Nearest Neighbour και ο Monotonic Relative-Neighborhood Graph, το καθένα με το δικό του αλγόριθμο αναζήτησης.

#### Το graph_search.cpp είναι το "κύριο" πρόγραμμα της εργασίας αυτής, όπου αυτή δέχεται τις διάφορες παραμέτρους. Ο κύριος ρόλος της είναι να καλέσει τον σωστό γράφο με βάση την παράμετρο -m. Για m=1 καλείται η GNNS_method (μέθοδος GNNS), και για m=2 η MRNG_method (μέθοδος MRNG).

#### Ο γράφος και για τις δύο μεθόδους αναπαρίσταται από λίστα γειτνίασης (με χρήση δισδιάστατου std::Vector), όπου για κάθε σημείο καταχωρούνται κόμβοι όσοι είναι και οι γείτονές του. Η δομή αυτή, μαζί με άλλες πληροφορίες (αριθμό εικόνων, pixles κλπ) που είναι κοινές και για τις δύο μεθόδους αποθηκεύονται στο object Graph_index, το οποίο είναι γονέας των GNNS και MRNG.

### Η GNNS μέθοδος,
+ επιπλέον κρατάει έναν πίνακα S με τους υποψήφιους πλησιέστερους γείτονες, τύπου point ώστε να ταξινομηθεί
+ κατά την κατασκευή απλά βρίσκει για κάθε σημείο τους k Πλησιέστερους Γείτονες χρησιμοποιώντας τον LSH και τους αποθηκεύει στον γράφο.
+ για την εύρεση πλησιέστερων, 
    + πρώτα βρίσκει τους υποψήφιους μέσω του GNNS αλγορίθμου αναζήτησης και τους αποθηκεύει στον S, καλώντας την συνάρτηση query,
    + και μετά καλείται η nearest_search, η οποία επιστρέφει τα N πρώτα σημεία του S, εφόσον ο S είναι ταξινομημένος με βάση την απόσταση από το Query

### Η MRNG μέθοδος,
+ επιπλέον κρατάει έναν πίνακα R με τους υποψήφιους πλησιέστερους γείτονες, τύπου candidate ώστε να ταξινομηθεί αλλά και να μαρκάρονται σημεία κατά την αναζήτηση. Κρατάει επίσης το start_node επειδή αυτό υπολογίζεται κατά την κατασκευή του γράφου.
+ κατά την κατασκευή, για κάθε σημείο p βρίκσεται ο (σχεδόν) πλησιέστερος t με LSH και αποθηκεύεται στον Lp. Τα υπόλοιπα σημεία r αποθηκεύονται στον Rp, ο οποίος σορτάρεται με βάση την απόσταση από το p. Έπειτα ξεκινάει η γνωστή διαδικασία. Ακριβώς μετά βρίσκεται το κεντροειδές σημείο του συνόλου σημείων.
+ για την εύρεση πλησιέστερων, 
    + πρώτα βρίσκει τους υποψήφιους μέσω του Search-On-Graph αλγορίθμου, με αρχή μονοπατιού το κεντροειδές σημείο, και τους αποθηκεύει στον R, καλώντας την συνάρτηση query,
    + και μετά καλείται η nearest_search, η οποία επιστρέφει τα N πρώτα σημεία του R, εφόσον ο R είναι ταξινομημένος με βάση την απόσταση από το Query

<br/>

### Επιπλέον πληροφορίες
+ Η μόνη διαφορά στις συναρτήσεις GNNS_method και MRNG_method είναι ο τύπος του object (GNNS ή MRNG). Παρόλα αυτά, το περιεχόμενο των συναρτήσεων αυτών δεν μπορούν να συνενωθούν στο graph_search.cpp, γιατί το object πρέπει να έχει εμβέλεια σε όλη τη main και όχι μόνο μέσα στην if όπου καλούνται οι συνατήσεις αυτές.\
Θα μπορούσε η graph_search.cpp, αντί να καλεί μία από τις δύο μεθόδους να αποθήκευε αντικείμενο τύπου GNNS ή MRNG στον δείκτη **Graph_index \*graph**, δηλωμένος έξω από την if (αυτός και ο σκοπός αυτής της προσέγγισης), όμως αυτό απαιτεί και οι τρεις κλάσεις GNNS, MRNG, Graph_Index να έχουν τις ίδιες συναρτήσεις, με ίδιες παραμέτρους, το οποίο δε βολεύει.
+ Η κατασκευή του MRNG παίρνει περίπου **15sec** για 1k εικόνες, **~1m7sec** για 2k, **~7m3sec** για 5k και **~28m12sec** για 10k.
+ Για μικρό dataset, είναι δυνατόν το E να είναι μεγαλύτερο από το σύνολο γειτόνων ενός σηείου. Υπάρχει αντίστοιχος έλεγχος στην add_neighbours() της GNNS.
+ Κατά την κατασκευή του MRNG, ο αρχικά πλησιέστερος ενός σημείου _p_ βρίσεκται με LSH. Για πολύ μικρο dataset (~100 εικόνες), επειδή τα σημεία που εξετάζονται είναι λίγα, μπορεί η LSH::query() να μην επιστρέψει κανέναν γείτονα αν το ID του _p_ διαφέρει από όλα τα λίγα αυτά σημεία.
+ Αν δεν γίνει σορτάρισμα στον Rp κατά τη κατασκευή του MRNG, πολλά σημεία που μπαίνουν στον Lp (άρα και οι γείτονες) είναι περιττά και μπορεί να χαλάνε την μονοτονία που υπόσχεται η μέθοδος.
+ Ο LSH, όπου χρησιμοποιείται, καλείται με w=150, K=4, L=5. Οι τιμές αυτές ισορροπούν την αποδοτικότητα με την αποτελεσματικότητα του αλγορίθμου.
+ Στην LSH::query, υπάρχει ένα όριο για το πόσα σημεία θα ελεγχθούν από τον κουβά που πέφτει το query. Για w=150, K=4, L=5 και 60k σημεία, τα συνολικά σημεία που βρίσκονται στον ίδιο κουβά με το query είναι 2000-3000, και η LSH::query υπολογίζει την απόσταση από αυτά τα σημεία με το query. Οπότε τελικά γίνονται 2000-3000 υπολογισμοί!\
Έχοντας ένα όριο στο πόσα σημεία να ελεγχθούν, η ακρίβεια αποτελεσμάτων πέφτει, αλλά εκτελείται πολύ πιο γρήγορα, πράγμα που είναι πολύ σημαντικό κατά τη κατασκευή του GNNS. Το όριο αυτό έχει επιλεχτεί να είναι 200*L, όπου L ο αριθμός των Hash Tables.
+ Ο GNNS αλγόριθμος αναζήτησης τερματίζει όταν το νέο σημείο μας απομακρύνει από το Query.
+ Στην γραμμή εντολών, αν δωθεί όρισμα -E μεγαλύτερο από -K, το E θα πάρει τη τιμή του K
+ Στην γραμμή εντολών, αν δωθεί όρισμα -l μικρότερο από -N, το l θα πάρει τη τιμή του N

<br/>


## Δομή Εργασίας <br/>


### Φάκελοι
+ include: header αρχεία (.h)
+ modules: Υλοποιήσεις των διάφορων modules
+ programs: Εκτελέσιμα προγράμματα. Στην εργασία αυτή ανήκει μόνο ο graph_index.cpp

### Αρχεία
+ graph_search.cpp: Το κύριο εκτελέσιμο πρόγραμμα, που διαβάζει παραμέτρους και τις στέλνει στην GNNS ή MRNG μέθοδο
+ graph_search_modl.cpp: module όπου υλοποιούνται οι μεθόδους GNNS/MRNG.
+ graph_search.h: Απετείται από **graph_search.cpp, graph_search_modl.cpp**

<br/>

+ gnns_modl.cpp: Υλοποίηση της μεθόδου GNNS και οποιεσδήποτε άλλες βοηθητικές συναρτήσεις
+ gnns.h: Απετείται από **gnns_modl.cpp, graph_search.h**

<br/>

+ mrng_modl.cpp: Υλοποίηση της μεθόδου MRNG και οποιεσδήποτε άλλες βοηθητικές συναρτήσεις
+ mrng.h: Απετείται από **mrng_modl.cpp, graph_search.h**

<br/>

+ graph_index_modl.cpp: Υλοποίηση της Graph_index (συγκεκριμένα την κατασκευή του μόνο), γονέας των GNNS/MRNG, και η υπλοποίηση της comp_points()
+ graph_index.h: Απετείται από **graph_index_modl.cpp, gnns.h, mrng.h**

<br/>

+ lsh_modl.cpp: Υλοποίηση της μεθόδου LSH
+ lsh.h: Απετείται από **lsh_modl.cpp, graph_index.h**

<br/>

+ methods_modl.cpp: Υλοποίηση της μεθόδου Method, γονέας των LSH/Hypercube
+ methods.h: Απετείται από **methods_modl.cpp, lsh.h**

<br/>

+ pr_modl.cpp: Υλοποίηση γενικών συναρτήσεων που χρησιμοποιούνται από οποιοδήποτε πρόγραμμα ή module (π.χ. υπολογισμός απόστασης μεταξύ σημείων), καθώς και μερικές μετονομασίες, structs και διάφορα .h αρχεία
+ pr.h: Απετείται από **pr_modl.cpp, methods.h**

<br/>

### Αρχεία (δενδρική μορφή)

pr.h\
|\
+-- pr_modl.cpp\
+-- methods.h\
&emsp;&ensp; |\
&emsp;&ensp; +-- methods_modl.cpp\
&emsp;&ensp; +-- lsh.h\
&emsp;&emsp;&emsp;&nbsp; |\
&emsp;&emsp;&emsp;&nbsp; +-- lsh_modl.cpp\
&emsp;&emsp;&emsp;&nbsp; +-- graph_index.h\
&emsp;&emsp;&emsp;&emsp;&emsp; |\
&emsp;&emsp;&emsp;&emsp;&emsp; +-- graph_index_modl.cpp\
&emsp;&emsp;&emsp;&emsp;&emsp; +-- gnns.h&emsp;<-------------- +\
&emsp;&emsp;&emsp;&emsp;&emsp; |&emsp;&emsp;|&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&emsp;&emsp;&emsp;&emsp;&nbsp; |\
&emsp;&emsp;&emsp;&emsp;&emsp; |&emsp;&emsp;+-- gnns_modl.cpp&emsp;&nbsp; |\
&emsp;&emsp;&emsp;&emsp;&emsp; +-- mrng.h&ensp; <-------------- +\
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; |&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;&emsp;&emsp;&emsp;&emsp;&nbsp; | \
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;+-- mrng_modl.cpp&emsp;&emsp;|\
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;&ensp;&ensp;+--graph_search.h \
&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;&emsp;|\
&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;&emsp;&ensp; +--graph_search_modl.cpp\
&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;&emsp;&ensp; +--graph_search.cpp

<br/>

## Μεταγλώττιση και Εκτέλεση
    /* Compile */
    make graph_search

    /* Clean Compiled Files */
    make graph_clean

    /* Run Program */
    ./graph_search –d <input file> –q <query file> –k <int> -E <int> -R <int> -N <int> -l <int, only for Search-on-Graph> -m <1 for GNNS, 2 for MRNG> -ο <output file>

### Παράδειγμα
    make graph_search
    ./graph_search -d ./archive/train-images.idx3-ubyte -q ./archive/t10k-images.idx3-ubyte -N 10 -m 2 -l 75 -o mrng_L75.txt
    make graph_clean
    make graph_search
    ./graph_search -d ./archive/train-images.idx3-ubyte -q ./archive/t10k-images.idx3-ubyte -N 10 -m 1 -k 20 -E 20 -R 1 -o gknn_K20E20R1.txt
