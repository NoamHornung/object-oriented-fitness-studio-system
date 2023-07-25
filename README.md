# object_oriented_gym_system
The fitness studio offers various workout plans, and they have talented trainers specializing in different workout options like Yoga, Pilates, CrossFit, etc. Each exercise can be either Cardio, Anaerobic or Mixed.

The C++ program will read a config file as input, containing information about the number of trainers, trainer capacities, and details about the available workout options. The program will then simulate the fitness studio management system and handle actions like opening and closing workout sessions, assigning customers to trainers, calculating trainer's salaries, moving customers between trainers, and printing various reports.

Each trainer can perform all the suggested workout types. There are four types of customers in this studio. Each customer type has a unique workout strategy. A customer may request a workout from a trainer more than once. Each trainer in the studio has a limited amount of available spots in their workout session (provided in the config file).

### The Program flow:
The program receives the path of the config file as the first command-line argument string. The config file contains the info about the number of trainers, the trainer’s capacities, and the workout that is available in the studio. Once the program starts, it prints "Studio is now open!" to the screen. Then the program waits for the user to enter an action to execute. After each executed action, the program waits for the next action in a loop. The program ends when the user enters the action "closeall".

The program will use several classes, including Studio, Trainer, Workout, Customer (with different ordering strategies), and BaseAction (for different action types). 
## The actions that can be performed by the user:
open a given trainer's workout session and assigns a list of customers- Syntax: open <trainer_id> <customer1>, < customer1_strategy> <customer2>, < customer2_strategy>. where the <customer_strategy>  is the 3-letter code for the ordering strategy.
making workout orders- When the customers enter the trainer's workout session. Syntax: order <trainer_id>
moving customers between trainers- Syntax: move <origin_trainer_id> <dest_trainer_id> <customer_id>
closing trainer sessions- Syntax: close <trainer_id>
printing available workout options of the studio- Syntax: workout_options
printing trainer status- Syntax: status <trainer_id>
printing the actions log- Syntax: log
BackupStudio – save all studio information in a global variable called “backup”. Syntax: backup
RestoreStudio – restore the backed-up studio status and overwrite the current studio status. Syntax: restore

### Ordering strategies:
Sweaty Customer- always orders all the cardio activities from the beginning of the work_options to their end (as is received in the input file). (3-letter code – swt).
Cheap Customer- always orders the cheapest workout from the workout options. This customer orders only once. (3-letter code – chp).
Heavy Muscle Customer- They always order all the anaerobic exercises from the most expensive to the cheapest. (3-letter code – mcl).
Full Body Customer- They always start with the cheapest cardio exercise offered, continue to the most expensive mix-type workout, and finish with the cheapest anaerobic exercise they can find. (3-letter code – fbd)


