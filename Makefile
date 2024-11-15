# Compilateur et options
CC = gcc
CFLAGS = -Wall

# Noms des fichiers exécutables
SERVER_EXEC = server
CLIENT_EXEC = client

# Liste des fichiers sources
SERVER_SRC = server.c
CLIENT_SRC = client.c
HANDLER_SRC = Handlers_Serv.h
COMMON_SRC = serv_cli_fifo.h

# Règle par défaut pour compiler tout
all: $(SERVER_EXEC) $(CLIENT_EXEC) fifos

# Compilation du serveur
$(SERVER_EXEC): $(SERVER_SRC) $(HANDLER_SRC) $(COMMON_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER_EXEC)

# Compilation du client
$(CLIENT_EXEC): $(CLIENT_SRC) $(COMMON_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT_EXEC)

# Création des tubes nommés FIFO
fifos:
	@echo "Création des tubes nommés FIFO..."
	@mkfifo -m 0666 /tmp/fifo1 || true
	@mkfifo -m 0666 /tmp/fifo2 || true

# Nettoyage des fichiers exécutables et des tubes nommés
clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC)
	rm -f /tmp/fifo1 /tmp/fifo2
	@echo "Nettoyage terminé."

# Règle pour exécuter le serveur et plusieurs clients dans des terminaux distincts
runz:
	./run_script.sh  # Exécute le script pour lancer les terminaux
