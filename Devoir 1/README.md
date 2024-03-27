# Simulation d'un système de gestion de tâches avec communication interprocessus  
  
Écrivez un programme en langage C sous Unix qui simule un système de gestion de tâches basique.   
Le programme doit inclure les fonctionnalités suivantes :  
1. Le processus principal crée un processus fils, appelé "gestionnaire de tâches".  
2. Le gestionnaire de tâches crée une file d'attente (par exemple, une pipe) pour recevoir
des demandes de tâches provenant d'autres processus.  
3. Le processus principal doit également installer un gestionnaire de signal pour capturer
le signal SIGINT (Ctrl+C).  
4. Lorsque le signal SIGINT est reçu par le processus principal, celui-ci doit envoyer un
signal SIGUSR1 au gestionnaire de tâches pour lui indiquer de terminer son exécution.  
5. Les autres processus (appelés "clients de tâches") peuvent envoyer des demandes de
tâches au gestionnaire de tâches via la file d'attente.  
6. Chaque demande de tâche doit être encapsulée dans une structure de données
appropriée, contenant au moins l'identifiant de la tâche et ses paramètres.  
7. Le gestionnaire de tâches doit traiter les demandes de tâches dans l'ordre d'arrivée et
exécuter chaque tâche dans un processus enfant séparé (utilisation de fork).  
8. Une fois qu'une tâche est terminée, le gestionnaire de tâches doit envoyer un message
de retour au client de tâches approprié pour signaler l'achèvement de la tâche.  
9. Le gestionnaire de tâches doit également gérer les cas où un processus enfant dépasse
un certain temps d'exécution et le tuer si nécessaire, en envoyant un signal approprié
au processus enfant.  
10. Assurez-vous que le programme gère correctement la terminaison de tous les
processus et nettoie les ressources allouées avant de se terminer.   
