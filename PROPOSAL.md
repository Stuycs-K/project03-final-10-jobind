# Final Project Proposal

## Group Members:

David Jobin
       
# Intentions:

I set out to create a signleplayer, civ-like strategy game, focused on diplomacy and natural resources.
The player controls a nation-state and competes against "AI" (Randomly controlled) opponent nations.
The goal is to demonstrate national superiority through one of three ways:
       By Eliminating all rival nations,
       By controlling a majority of the world's GDP,
       or By creating a world-dominating military alliance.
    
# Intended usage:

Each player turn will represent a year of running their country.
Play will be divided into phases, each dedicated to controlling certain aspects of the nation,
such as the economy, alliances with others, and war.
The gameplay will consist of typing into the prompt and looking at numbers in charts.
  
# Technical Details:

Each country will be run with forking processes, with pipes acting as communication
between the different nations.
Files will be used to store regional data, and semaphores will be used to limit troop presence during
conflicts.

# Intended pacing:

1/7: Completion of the structures needed to handle nations and their domestic actions.
1/9: Completion of diplomatic features.
1/11: Completion of war mechanics and land seizure.
1/16: Fine-tuning of gameplay complete.

# Back-up plan for downsizing (Just in case)

Singleplayer nation sim.