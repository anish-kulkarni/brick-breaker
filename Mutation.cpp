
///////////// Mutation Pseudo-code /////////////

/// Ensure infinite loop selecting connection

bool in_organism(int x,int y)
{
    for(int i=0; i < next_gen.offspring.back().connection_genes.size(); i++)
    {
        if(next_gen.offspring.back().connection_genes[i].start == x && next_gen.offspring.back().connection_genes[i].end == y)
            return true;
    }
    return false;
}

int in_connection_pool(int x,int y)
{
    for(int i = 0; i < next_gen.connection_gene_pool.size(); i++)
    {
        if(next_gen.connection_gene_pool[i].start==x && next_gen.connection_gene_pool[i].end==y)
            return next_gen.connection_gene_pool[i].innovation_number;
    }

    return 0;
}

int in_node_pool(int x)
{
    for(int i = 0; i < next_gen.node_gene_pool.size(); i++)
    {
        if(next_gen.node_gene_pool[i].broken_connection_number==x)
            return next_gen.node_gene_pool[i].innovation_number;
    }

    return 0;
}

void add_connection_gene_pool(int x, int y, int innovation_number)
{
    next_gen.connection_gene_pool.push_back(Connection_pool(x,y,innoavtion_number));
}

void add_node_gene_pool(int x, int innovation_number)
{
    next_gen.node_gene_pool.push_back(Node_pool(x,innoavtion_number));
}

void mutate_weight(int members, vector<int> mutate[], bool repeat_mutate)
{
    for(int i = 0 ; i < members; i++)
    {
        // Random select Connection to Mutrate
        if(!repeat_mutate)
            next_gen.offspring.push_back(curr_gen.offspring[mutate[i]]);
        int x = random_number(0,next_gen.offspring.back().genome.connection_genes.size()-1);

        // Mutate connection weight
        if(probability(probability_weight_tweak))
            if(next_gen.offspring.back().genome.connection_genes[x].weight > weight_limit - delta_weight)
                next_gen.offspring.back().genome.connection_genes[x].weight -= delta_weight;
            else if(next_gen.offspring.back().genome.connection_genes[x].weight < -weight_limit + delta_weight)
                next_gen.offspring.back().genome.connection_genes[x].weight += delta_weight;
            else if(probability(0.5)) // Tweak Weight
                next_gen.offspring.back().genome.connection_genes[x].weight += delta_weight;
            else
                next_gen.offspring.back().genome.connection_genes[x].weight -= delta_weight;
        else
            next_gen.offspring.back().genome.connection_genes[x].weight = random_number(-weight_limit,weight_limit); // Random Weight assign
    }
}

void mutate_gene(int members, vector<int> mutate[], bool repeat_mutate)
{
    for(int i = 0; i < members; i++)
    {
        // Random select nodes for connection
        next_gen.offspring.push_back(curr_gen.offspring[mutate[i]]);
        int x,y,innovation,n = next_gen.offspring.back().genome.node_genes.size();
        int passes = 0;
        bool pass_break=false;
        for(;;)
        {
            passes++;
            if(passes > times_pass_check)
                return;
            for(;;)
            {
                x = random_number(0,n-1);
                if(next_gen.offspring.back().genome.node_genes[x].type != 1)
                    pass_break=true;
            }
            if(!pass_break)
                continue;
            for(;;)
            {
                y = random_number(x+1,n-1);
                if(y!=x && next_gen.offspring.back().genome.node_genes[x].type != -1)
                    pass_break=true;
            }
            if(!pass_break)
                continue;
            if(!in_organism(next_gen.offspring.back().node_genes[x].innovation_number,next_gen.offspring.back().node_genes[y].innovation_number))
                break;

        }

        //  Connection
        int innovation = in_connection_pool(next_gen.offspring.back().node_genes[x].innovation_number,next_gen.offspring.back().node_genes[y].innovation_number);
        if(innovation > 0)
        {
            next_gen.offspring.back().genome.connection_genes.push_back(Connection_gene(next_gen.offspring.back().node_genes[x].innovation_number,next_gen.offspring.back().node_genes[y].innovation_number,random_number(-weight_limit,weight_limit),innovation));
        }
        else
        {
            global_connection_innovation_number++;
            next_gen.offspring.back().genome.connection_genes.push_back(Connection_gene(next_gen.offspring.back().node_genes[x].innovation_number,next_gen.offspring.back().node_genes[y].innovation_number,random_number(-weight_limit,weight_limit),global_connection_innovation_number));
            add_connection_gene_pool(next_gen.offspring.back().node_genes[x].innovation_number,next_gen.offspring.back().node_genes[y].innovation_number,global_connection_innovation_number);
        }
    }

    if(repeat_mutate)
        mutate_weight(members , mutate[] , repeat_mutate);
}

void mutate_node(int members, vector<int> mutate[], bool repeat_mutate)
{
    for(int i = 0; i < members; i++)
    {
        // Random select connection to Mutate
        next_gen.offspring.push_back(curr_gen.offspring[mutate[i]]);
        int x;
        x = random_number(0,next_gen.offspring.back().connection_genes.size());

        int input = next_gen.offspring.back().connection_genes(x).input;
        int output = next_gen.offspring.back().connection_genes(x).output;
        double weight = next_gen.offspring.back().connection_genes(x).weight;

        // Create new node with innovation number

        int position = 0;
        for(position = 0; position < next_gen.offspring.back().node_genes.size() ; position++)
            if(next_gen.offspring.back().node_genes[position].innovation_number)
                break;

        int innovation = in_node_pool(next_gen.offspring.back().connection_genes(x).innovation_number);
        if(innovation > 0)
        {
            next_gen.offspring.back().node_genes.push_back(position,Node_gene(0,innovation_number));
            innovation = in_connection_pool(next_gen.offspring.back().connection_genes(x).input,next_gen.offspring.back().connection_genes(x).output);
        }
        else
        {
            global_node_innovation_number++;
            next_gen.offspring.back().node_genes.push_back(position,Node_gene(0,global_node_innovation_number);
            add_node_gene_pool(next_gen.offspring.back().connection_genes(x).innovation_number,global_node_innovation_number);
        }

        // Add node to connection
        next_gen.offspring.back().connection_genes(x).enabled = false;
        int k = next_gen.offspring.back().node_genes.back().innovation_number;

        if(innovation > 0)
        {
            next_gen.offspring.back().genome.connection_genes.push_back(Connection_gene(input,k,1,innovation));
        }
        else
        {
            global_connection_innovation_number++;
            next_gen.offspring.back().genome.connection_genes.push_back(Connection_gene(input,k,1,global_connection_innovation_number));
            add_connection_gene_pool(input,k,global_connection_innovation_number);
        }

        if(innovation > 0)
        {
            next_gen.offspring.back().genome.connection_genes.push_back(Connection_gene(k,output,weight,innovation+1));
        }
        else
        {
            global_connection_innovation_number++;
            next_gen.offspring.back().genome.connection_genes.push_back(Connection_gene(k,output,weight,global_connection_innovation_number));
            add_connection_gene_pool(k,output,global_connection_innovation_number);
        }
    }

    if(repeat_mutate)
        mutate_weight(members , mutate[] , repeat_mutate);
}

void mutaion_species(vector<int> mutate[],int offspring)
{
    int x,members = mutate.size();
    vector<int> to_mutate;
    for(;;)
    {
        // Select Organisms to be Mutated Randomly
        x = random_number(0,members-1);
        if(x not in to_mutate) /// how to do this
        {
            to_mutate.push_back(x);
            if(to_mutate.size()>= offspring*100 / percent_mutate)
                break;
        }
    }

    // Mutate Oragnism weights
    int x1 = to_mutate.size()*100 / percent_weight_mutation;
    vector<int> to_mutate_weights;
    for(int i = 0; i < x1; i++)
    {
        to_mutate_weights.push_back(to_mutate.back());
        to_mutate.pop_back();
    }
    mutate_weight(to_mutate_weights.size(), to_mutate_weights[], false);

    // Mutate Orgamism by Adding Genes
    int x2 = to_mutate.size()*100 / percent_gene_mutation;
    vector<int> to_mutate_genes;
    for(int i = x1; i < x2; i++)
    {
        to_mutate_genes.push_back(to_mutate.back());
        to_mutate.pop_back();
    }
    mutate_gene(to_mutate_genes.size(), to_mutate_genes[], false);

    // Mutate Organism by Adding Node
    int x3 = to_mutate.size()*100 / percent_node_mutation;
    vector<int> to_mutate_nodes;
    for(int i = x2; i < x3; i++)
    {
        to_mutate_nodes.push_back(to_mutate.back());
        to_mutate.pop_back();
    }
    mutate_node(to_mutate_nodes.size(), to_mutate_nodes[], false);

    // Combined Mutation Pending

    // Mutate Orgamism by Adding Genes and then mutate weights
    int x4 = to_mutate.size()*100 / percent_gene_mutation;
    vector<int> to_mutate_genes_weights;
    for(int i = x3; i < x4; i++)
    {
        to_mutate_genes_weights.push_back(to_mutate.back());
        to_mutate.pop_back();
    }
    mutate_gene(to_mutate_genes_weights.size(), to_mutate_genes_weights[], true);

    // Mutate Organism by Adding Node and then mutate weights
    vector<int> to_mutate_nodes_weights;
    for(;;)
    {
        if(to_mutate.size()==0)
            break;
        to_mutate_nodes_weights.push_back(to_mutate.back());
        to_mutate.pop_back();
    }
    mutate_node(to_mutate_nodes_weights.size(), to_mutate_nodes_weights[], true);
}

// This is the main_mutaion function which will be called for mutation
void mutation()
{
    for(int i = 0; i < curr_gen.species.size(); i++)
    {
        mutaion_species(curr_gen.species[i].individual[],curr_gen.species[i].permitted_mutants);
    }
}
