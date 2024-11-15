#!/bin/bash

# Nombre de clients à lancer
NUM_CLIENTS=4

# Lancer le serveur dans une console séparée
gnome-terminal -- bash -i -c "./server; exec bash" &  # Utilisation de bash -i pour maintenir le terminal ouvert

# Attendre que le serveur soit prêt
sleep 2

# Lancer chaque client dans une nouvelle console
for ((i=1; i<=NUM_CLIENTS; i++))
do
    echo "Lancement du client $i"
    gnome-terminal -- bash -i -c "./client; exec bash" &  # Utilisation de bash -i pour maintenir le terminal ouvert
done
