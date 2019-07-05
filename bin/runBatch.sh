cleanup ()
{
    kill -s SIGTERM $!
    exit 0
}

trap cleanup SIGINT SIGTERM
for mutation in {1..4}
do
    for crossover in {1..5}
    do
        for reset in {1..3}
        do
            mut=$(awk 'BEGIN{printf "%f", '$mutation' / 6}')
            co=$(awk 'BEGIN{printf "%f", '$crossover' / 14}')
            re=$(awk 'BEGIN{printf "%f", '$reset' / 50}')
            num=$(awk 'BEGIN{printf "%i", 15*('$mutation'-1)+3*('$crossover'-1)+'$reset'-1}')

            echo "$num - MUT: $mut - CO: $co - RE: $re"
            #cp ../config.ini ../config.backup.ini

            #sed -i "s/MUTATION_RATE		= 0.2/MUTATION_RATE		= $mut/g" ../config.ini
            #sed -i "s/CROSSOVER_RATE		= 0.1/CROSSOVER_RATE		= $co/g" ../config.ini
            #sed -i "s/RESET_RATE			= 0.02/RESET_RATE			= $re/g" ../config.ini
            #sed -i "s/BATCH_NUM           = 0/BATCH_NUM           = $num/g" ../config.ini
            
            #mpirun -n 4 ./ga-dao & wait $!

            #cp ../config.backup.ini ../config.ini
        done
    done
done