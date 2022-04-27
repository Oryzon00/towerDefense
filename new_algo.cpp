#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include <vector>
#include <cstdio>
#include <math.h>

using namespace std;

int	calculate_distance(int x1, int y1, int x2, int y2);

# define SUCCESS 0
# define FAILURE (!SUCCESS)
# define EARLY 70

typedef	struct	s_was_control
{
	int	hero_0;
	int	hero_1;
	int	hero_2;
}	t_was_control;

typedef	struct s_general
{
	int				base_x;
	int				base_y;
	int				nb_heroes;
	int				health;
	int				mana;
	int				entity_count;
	int				enemy_health;
	int				enemy_mana;
	t_was_control	was_control;
	int				hero_num;
	int				tours;
}	t_general;


typedef	struct s_entity
{
	int	id;
	int	type;
	int	x;
	int	y;
	int	shield_life;
	int	is_controlled;
	int	health;
	int	vx;
	int	vy;
	int	near_base;
	int	threat_for;
    int distance;
}	t_entity;

typedef struct s_coor
{
	int	x;
	int	y;
	int	vx;
	int	vy;
}	t_coor;

typedef struct s_threat
{
	int	near_base;
	int	threat_for;
	int	distance;
}	t_threat;

typedef struct s_monstre
{
	int			id;
	int			is_controlled;
	int			shield_life;
	int 		health;
	t_coor 		coor;
	t_threat	threat;
	int			is_focus;
}	t_monstre;

typedef struct s_hero
{
	t_coor		coor;
	int			id;
	int			is_controlled;
	int			was_control;
	int			shield_life;
	t_threat	threat;
}	t_hero;

typedef struct s_enemy
{
	t_coor		coor;
	int			id;
	int			is_controlled;
	int			shield_life;
	t_threat	threat;

}	t_enemy;

t_general	*_general(void)
{
	static t_general	general;
	static int			check = 0;

	if (check == 0)
	{
		check = 1;
		bzero(&general, sizeof(t_general));
	}
	return (&general);
}

vector <t_entity>	*_entity(void)
{
	static vector <t_entity> 	entity;
	static int					check = 0;

	if (check == 0)
	{
		check = 1;
		bzero(&entity, sizeof(vector <t_entity>));
	}
	return (&entity);
}

vector <t_monstre>	*_monstre(void)
{
	static vector <t_monstre>	monstre;
	static	int					check = 0;

	if (check == 0)
	{
		check = 1;
		bzero(&monstre, sizeof(vector <t_monstre>));
	}
	return (&monstre);
}

vector <t_enemy>	*_enemy(void)
{
	static	vector <t_enemy>	enemy;
	static	int					check = 0;

	if (check == 0)
	{
		check = 1;
		bzero(&enemy, sizeof(vector <t_enemy>));
	}
	return (&enemy);
}

vector <t_hero>	*_hero(void)
{
	static	vector <t_hero>	hero;
	static	int				check = 0;

	if (check == 0)
	{
		check = 1;
		bzero(&hero, sizeof(vector <t_hero>));
	}
	return (&hero);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	fill_general(void)
{
	t_general	*general;

	general = _general();
    scanf("%d%d", &general->base_x, &general->base_y);
    scanf("%d", &general->nb_heroes);
	general->was_control.hero_0 = false;
	general->was_control.hero_1 = false;
	general->was_control.hero_2 = false;
}

void	update_general(int	tours)
{
	t_general	*general;
	int health;
	int mana;

	general = _general();
	for (int i = 0; i < 2; i++) 
	{

		scanf("%d%d", &health, &mana);
		if (i == 0)
		{
			general->health = health;
			general->mana = mana;
		}
		else if (i == 1)
		{
			general->enemy_health = health;
			general->enemy_mana = mana;
		}
	}
	scanf("%d", &general->entity_count);
	general->tours = tours;

}

void	update_entity(void)
{
	t_general	*general;

	general = _general();
	for (int i = 0; i < general->entity_count; i++)
	{
		t_entity    		entity;
		int         		found = 0;
		vector <t_entity>	*list;

		list = _entity();
		scanf("%d%d%d%d%d%d%d%d%d%d%d", &entity.id, &entity.type, &entity.x, &entity.y, &entity.shield_life, 
			&entity.is_controlled, &entity.health, &entity.vx, &entity.vy, &entity.near_base, &entity.threat_for);

		entity.distance = calculate_distance(entity.x, entity.y, general->base_x, general->base_y);
		list->push_back(entity);
	}
}

void	reset_singleton(void)
{
	vector	<t_entity>	*entity;
	vector 	<t_monstre>	*monstre;
	vector	<t_hero>	*hero;
	vector	<t_enemy>	*enemy;

	entity = _entity();
	monstre = _monstre();
	hero = _hero();
	enemy = _enemy();

	entity->clear();
	monstre->clear();
	hero->clear();
	enemy->clear();
}

void	fill_monster_list(t_entity entity)
{
	vector <t_monstre>	*list_monstre;
	t_monstre			monstre;

	list_monstre = _monstre();
	monstre.coor.x = entity.x;
	monstre.coor.y = entity.y;
	monstre.coor.vx = entity.vx;
	monstre.coor.vy = entity.vy;
	monstre.health = entity.health;
	monstre.id = entity.id;
	monstre.is_controlled = entity.is_controlled;
	monstre.shield_life = entity.shield_life;
	monstre.threat.distance = entity.distance;
	monstre.threat.near_base = entity.near_base;
	monstre.threat.threat_for = entity.threat_for;
	monstre.is_focus = false;
	list_monstre->push_back(monstre);
}

void	fill_hero_list(t_entity entity)
{
	vector <t_hero>	*list_hero;
	t_hero			hero;

	list_hero = _hero();
	hero.coor.x = entity.x;
	hero.coor.y = entity.y;
	hero.id = entity.id;
	hero.is_controlled = entity.is_controlled;
	hero.shield_life = entity.shield_life;
	hero.threat.distance = entity.distance;
	list_hero->push_back(hero);
}

void	fill_enemy_list(t_entity entity)
{
	vector <t_enemy>	*list_enemy;
	t_enemy			enemy;

	list_enemy = _enemy();
	enemy.coor.x = entity.x;
	enemy.coor.y = entity.y;
	enemy.id = entity.id;
	enemy.is_controlled = entity.is_controlled;
	enemy.shield_life = entity.shield_life;
	enemy.threat.distance = entity.distance;
	list_enemy->push_back(enemy);
}

void	update_was_control(void)
{
	t_general			*general;
	vector <t_hero>		*hero;

	general = _general();
	hero = _hero();
	for	(int i = 0; i < 3; i++)
	{
		if ((*hero)[0].is_controlled == 1)
			general->was_control.hero_0 = true;
		else if ((*hero)[1].is_controlled == 1)
			general->was_control.hero_1 = true;
		else if ((*hero)[2].is_controlled == 1)
			general->was_control.hero_2 = true;
	}
	(*hero)[0].was_control = general->was_control.hero_0;
	(*hero)[1].was_control = general->was_control.hero_1;
	(*hero)[2].was_control = general->was_control.hero_2;
 
}

void	update_data(void)
{
	vector <t_entity>	*list;

	list = _entity();
	for (int i = 0; i < list->size(); i++)
	{
		if ((*list)[i].type == 0)
			fill_monster_list((*list)[i]);
		else if ((*list)[i].type == 1)
			fill_hero_list ((*list)[i]);
		else if ((*list)[i].type == 2)
			fill_enemy_list((*list)[i]);
	}
	update_was_control();
}

void	update_hero_number(int	hero_num)
{
	t_general	*general;

	general = _general();
	general->hero_num = hero_num;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int	cast_shield(void) //ajouter 3000 de distance
{
	vector <t_hero>	*hero_list;
	t_general		*general;
	t_hero			hero;

	general = _general();
	hero_list = _hero();
	hero = (*hero_list)[general->hero_num];

	if (general->mana < 10)
	{
		dprintf(2, "out of mana\n");
		return (FAILURE);
	}
	if (hero.was_control == true && hero.shield_life == 0)
	{
		printf("SHIELD %d", hero.id);
		general->mana -= 10;
		return (SUCCESS);
	}
	return (FAILURE);
}

void	move_to_def_pos(t_general *general)
{
	int	x = 700;
	int	y = 700;
	if (general->base_x == 0)
		printf("MOVE %d %d\n", x, y);
	else
		printf("MOVE %d %d\n", 17630 - x, 9000 - y);
}

void	move_to_wait_pos(t_general *general, int hero)
{
	int	x;
	int	y;

	if (hero == 0)
	{
		x = 700;
		y = 700;
	}
	else if (hero == 1)
	{
		x = 7000;
		y = 2500;
	}
	else if (hero == 2)
	{
		x = 4000;
		y = 5500;
	}
	if (general->base_x == 0)
		printf("MOVE %d %d\n", x, y);
	else
		printf("MOVE %d %d\n", 17630 - x, 9000 - y);
}

void	move_to_wait_pos2(t_general *general)
{
	int	x = 7500;
	int	y = 1000;
	if (general->base_x == 0)
		printf("MOVE %d %d\n", x, y);
	else
		printf("MOVE %d %d\n", 17630 - x, 9000 - y);
}

/* 
void	move_to_target(t_entity *target, t_general *general, int hero)
{
    if (target->type == -1)
        move_to_wait_pos(general, hero);
    else
	    printf("MOVE %d %d\n", target->x + target->vx, target->y + target->vy);
	dprintf(2, "target x = %d, target y = %d\n", target->x, target->y);
}
 */


void    calculate_distance_to_base(t_entity &entity, t_general *general)
{
     if (entity.threat_for == 1)
        entity.distance = sqrt(pow((entity.x - general->base_x), 2) + pow((entity.y - general->base_y), 2));
}

int	calculate_distance(int x1, int y1, int x2, int y2)
{
	int distance;

	distance = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
	return (distance);
}



/* void    choose_target(vector <t_entity> *list, t_general *general, t_entity *target)
{
	target->type = -1;
    target->distance = 0;
	for (int i = 0; i < list->size(); i++)
    {
        if ((*list)[i].type == 0 && (*list)[i].distance != -1)
        {
            *target = (*list)[i];
            break ;
        }
    }

    for (int i = 0; i < (*list).size(); i++)
    {

        if ((*list)[i].distance > 0 && ((*list)[i].distance < target->distance || target->distance == 0) && (*list)[i].health > 0)
            *target = (*list)[i];
    }
} */


int	check_base_danger(vector <t_entity> *list, t_general *general) ///////////////////A reecrire
{
	for (int i = 0; i < list->size(); i++)
	{
		if ((*list)[i].distance <= 700 && (*list)[i].type == 0 && (*list)[i].threat_for == 1)
		{
			return (FAILURE);
		}
	}
	return (SUCCESS);
}


void	move_to(int x, int y)
{
	printf("MOVE %d %d\n", x, y);
}

int	hit_closest_ennemy(vector <t_entity> &list, t_general *general) //////////////////////////////////////A CODER
{
	for (int i = 0; i < list.size(); i++)
	{

	}
}

/* //Defenseur
void	hero_0(vector <t_entity> *list, t_general *general, int hero)
{
	if (check_base_danger(list, general) == FAILURE)
		printf("SPELL WIND %d %d\n", 17630 - general->base_x, 9000 - general->base_y);
	else
		move_to_def_pos(general);
}

void	hero_1(vector <t_entity> *list, t_general *general, int hero)
{
	t_entity target;

	choose_target(list, general, &target);
    move_to_target(&target, general, hero);
}

void	hero_2(vector <t_entity> *list, t_general *general, int hero)
{	
	t_entity target;

	choose_target(list, general, &target);
    move_to_target(&target, general, hero);
} */

/* void	calculate_threat_all(vector <t_entity> *list, t_general *general)
{
	for (int i = 0; i < list->size(); i++)
	{
		if ((*list)[i].type == 0)
            calculate_distance_to_base((*list)[i], general);
	}
} */

void	defenseur(t_general	*general)
{
	
}

//pos early_defense 7500, 1500

void	move_early_def_pos(int x, int y, t_general *general)
{
	if (general->base_x == 0)
		move_to(x, y);
	else
		move_to(general->base_x - x, general->base_y - y);
}

void	move_to_monstre(t_monstre monstre)
{
	int	x;
	int	y;

	x = monstre.coor.x + monstre.coor.vx;
	y = monstre.coor.y + monstre.coor.vy;
	move_to(x, y);
}

int	attack_monster_in_base(t_general *general)
{
	vector <t_monstre> *list;

	list = _monstre();
	for (int i = 0; i < list->size(); i++)
	{
		if ((*list)[i].threat.near_base == 1)
		{
			move_to_monstre((*list)[i]);
			return (SUCCESS);
		}
	}
	return (FAILURE);
}

int	in_circle(int x_centre, int y_centre, int x, int y, int rayon)
{
	if (pow((x - x_centre), 2) + pow((y - y_centre), 2) < pow(rayon, 2))
		return (SUCCESS);
	return (FAILURE);
}

//N'attaque PAS le plus proche
int	attack_monster_in_range(int x_centre, int y_centre, t_general *general) 
{
	vector <t_monstre>	*list;
	t_monstre			monstre;
	int					rayon;

	list = _monstre();
	rayon = 2000;
	for (int i = 0; i < list->size(); i++)
	{
		monstre = (*list)[i];
		if (in_circle(x_centre, y_centre, monstre.coor.x, monstre.coor.y, rayon) == SUCCESS
			&& (monstre.is_focus == false))
		{
			move_to_monstre(monstre);
			monstre.is_focus = true;
			return (SUCCESS);
		}
	}
	return (FAILURE);
}

//fonction qui rend une coor par rapport a la position de la base

void	early_algo_defense(t_general *general)
{
	int	x = abs(general->base_x - 7500);
	int	y = abs(general->base_y - 1500);

	if(attack_monster_in_base(general) == SUCCESS)
		return ;
	else if (attack_monster_in_range(x, y, general))
		return ;
	else
		move_early_def_pos(x, y, general);

}

void	hero_0(t_general *general)
{
	if (general->tours < EARLY)
		early_algo_defense(general);
	else if (cast_shield() == SUCCESS)
		return ;
	else
		defenseur(general);

}


void	action_algo(t_general *general)
{
	int	hero_num = general->hero_num;

	if (hero_num == 0)
		hero_0(general);
	else if (hero_num == 1)
	{

	}
	else if (hero_num == 2)
	{

	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main()
{
	t_general	*general;
	int	tours = 1;

	fill_general();
	general = _general();

    while (tours++) 
	{
		update_general(tours);
		update_entity();
		update_data();
        for (int hero = 0; hero < general->nb_heroes; hero++)
		{
			update_hero_number(hero);
			action_algo(general);
		}
		reset_singleton();
    }
    return 0;
}
