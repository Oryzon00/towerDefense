#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include <vector>
#include <cstdio>
#include <math.h>

using namespace std;

# define SUCCESS 0
# define FAILURE (!SUCCESS)
# define EARLY 230
# define EARLY_HERO_0 25
# define EARLY_HERO_1 45
# define EARLY_HERO_2 45

typedef	struct	s_was_control
{
	int	hero_0;
	int	hero_1;
	int	hero_2;
}	t_was_control;

typedef struct	s_atk
{
	int set_up;
	int	patrouille;
}	t_atk;

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
	int				cannon_ready;
	t_atk			atk;			
}	t_general;

typedef struct	s_patrouille
{
	int	xA;
	int yA;
	int xB;;
	int yB;
}	t_patrouille;


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
	int			focus_by;
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

int	calculate_distance(int x1, int y1, int x2, int y2)
{
	int distance;

	distance = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
	return (distance);
}

int	calculate_distance_to_base(t_general *general, int x, int y)
{
	int	distance_base;

	distance_base = (calculate_distance(general->base_x, general->base_y, x, y));
	return (distance_base);
}

int	calculate_distance_to_enemy(t_general *general, int x, int y)
{
	int	distance_base;

	distance_base = (calculate_distance(17630 - general->base_x, 9000 - general->base_y, x, y));
	return (distance_base);
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
	general->atk.set_up = false;
	general->atk.patrouille = 0;
	general->cannon_ready = false;
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

		entity.distance = calculate_distance_to_base(general, entity.x, entity.y);
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

void	get_true_coor(t_general *general, int *x, int *y)
{
	*x = abs(general->base_x - *x);
	*y = abs(general->base_y - *y);
}

void	move_to(int x, int y)
{
	printf("MOVE %d %d\n", x, y);
}

void	move_to_monstre(t_monstre monstre)
{
	int	x;
	int	y;

	x = monstre.coor.x + monstre.coor.vx;
	y = monstre.coor.y + monstre.coor.vy;
	move_to(x, y);
}

int	in_circle(int x_centre, int y_centre, int x, int y, int rayon)
{
	if (pow((x - x_centre), 2) + pow((y - y_centre), 2) < pow(rayon, 2))
		return (SUCCESS);
	return (FAILURE);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int	hit_closest_to_base(t_general *general)
{
	vector <t_monstre>	*list;
	t_monstre			target;
	t_monstre			monstre;

	list = _monstre();
	if (list->size() == 0)
		return (FAILURE);
	target = (*list)[0];
	for (int i = 0; i < list->size(); i++)
	{
		monstre = (*list)[i];
		if (calculate_distance_to_base(general, monstre.coor.x, monstre.coor.y) <
			calculate_distance_to_base(general, target.coor.x, target.coor.y))
			target = monstre;
	}
	move_to_monstre(target);
	return (SUCCESS);
}


int	no_spider_in_range(t_general *general, int x_centre, int y_centre, int rayon)
{
	vector <t_monstre>	*list;
	t_monstre			monstre;
	list = _monstre();

	if (list->size() == 0)
		return (FAILURE);
	for (int i = 0; i < list->size(); i++)
	{
		monstre = (*list)[i];
		if ((in_circle(x_centre, y_centre, monstre.coor.x, monstre.coor.y, rayon) == SUCCESS))
			return (SUCCESS);
	}
	return (FAILURE);
}

int	hit_closest_in_range(t_general *general, int x_centre, int y_centre, int rayon) 
{
	vector <t_monstre>	*list;
	t_monstre			monstre;
	t_monstre			target;
	vector	<t_hero>	*hero_list;
	t_hero				hero;

	list = _monstre();
	hero_list = _hero();
	if (no_spider_in_range(general, x_centre, y_centre, rayon) == FAILURE)
		return (FAILURE);
	hero = (*hero_list)[general->hero_num];
	target = (*list)[0];
	for (int i = 0; i < list->size(); i++)
	{
		monstre = (*list)[i];
		if ((in_circle(x_centre, y_centre, monstre.coor.x, monstre.coor.y, rayon) == SUCCESS)
			&& (monstre.is_focus == false)
			&& (calculate_distance(monstre.coor.x, monstre.coor.y, hero.coor.x, hero.coor.y) < 
			calculate_distance(target.coor.x, target.coor.y, hero.coor.x, hero.coor.y)))
		{
			target = monstre;
			monstre.is_focus = true;
		}
	}
	move_to_monstre(target);
	return (SUCCESS);
}

int	hit_closest(t_general *general)
{
	vector	<t_monstre>	*list;
	t_monstre			target;
	t_monstre			monstre;
	vector	<t_hero>	*hero_list;
	t_hero				hero;

	list = _monstre();
	hero_list = _hero();
	if (list->size() == 0)
		return (FAILURE);
	hero = (*hero_list)[general->hero_num];
	target = (*list)[0];
	for (int i = 0; i < list->size(); i++)
	{
		monstre = (*list)[i];
		if (calculate_distance(monstre.coor.x, monstre.coor.y, hero.coor.x, hero.coor.y) < 
			calculate_distance(target.coor.x, target.coor.y, hero.coor.x, hero.coor.y))
			target = monstre;
	}
	move_to_monstre(target);
	return (SUCCESS);
}

int	cast_shield(void)
{
	vector <t_hero>		*hero_list;
	vector <t_enemy>	*enemy_list;
	t_general		*general;
	t_hero			hero;
	t_enemy			enemy;
	int				danger;

	general = _general();
	hero_list = _hero();
	enemy_list = _enemy();
	hero = (*hero_list)[general->hero_num];
	danger = 0;


	for (int i = 0; i < enemy_list->size(); i++)
	{
		enemy = (*enemy_list)[i];
		if (calculate_distance(enemy.coor.x, enemy.coor.y, hero.coor.x, hero.coor.y) <= 2200)
			danger = 1;
	}
	if (danger == 1 && hero.was_control == true && (hero.shield_life == 0)) //attention
	{
		if (general->mana < 10)
		{
			dprintf(2, "out of mana\n");
			return (FAILURE);
		}
		else
		{
			printf("SPELL SHIELD %d\n", hero.id);
			general->mana -= 10;
			return (SUCCESS);
		}
	}
	return (FAILURE);
}

int	attack_monster_in_base(t_general *general)
{
	vector <t_monstre> *list;

	list = _monstre();
	for (int i = 0; i < list->size(); i++)
	{
		if ((*list)[i].threat.near_base == 1)
		{
			hit_closest_to_base(general);
			return (SUCCESS);
		}
	}
	return (FAILURE);
}

void	wind_to_enemy_base(t_general *general)
{
	printf("SPELL WIND %d %d\n", 17630 - general->base_x, 9000 - general->base_y);
}

int	wind_defense(t_general *general)
{
	vector	<t_monstre> *list;
	t_monstre			monstre;

	list = _monstre();
	for (int i = 0; i < list->size(); i++)
	{
		monstre = (*list)[i];
		if (monstre.threat.distance <= 700)
		{
			wind_to_enemy_base(general);
			return (SUCCESS);
		}
	}
	return (FAILURE);
}

void	early_algo_hero_0(t_general *general)
{
	int	x = 9000;
	int	y = 1500;
	int range = 2500;

	get_true_coor(general, &x, &y);
	if (wind_defense(general) == SUCCESS)
		return ;
	else if(attack_monster_in_base(general) == SUCCESS)
		return ;
	else if (hit_closest_in_range(general, x, y, range) == SUCCESS)
		return ;
	else
		move_to(x, y);

}

void	early_algo_hero_1(t_general *general)
{
	int	x = 4800;
	int	y = 7500;
	int	range = 2500;

	get_true_coor(general, &x, &y);
/* 	if(attack_monster_in_base(general) == SUCCESS)
		return ;
	else  */if (hit_closest_in_range(general, x, y, range) == SUCCESS)
		return ;
	else
		move_to(x, y);

}

void	early_algo_hero_2(t_general *general)
{
	int	x = 8500;
	int	y = 7500;
	int range = 2500;

	get_true_coor(general, &x, &y);
/* 	if(attack_monster_in_base(general) == SUCCESS)
		return ;
	else */ if (hit_closest_in_range(general, x, y, range) == SUCCESS)
		return ;
	else
		move_to(x, y);

}

void	defenseur(t_general *general)
{
	int x = 1500;
	int y = 1500;
	int range = 2000;

	get_true_coor(general, &x, &y);
	if (wind_defense(general) == SUCCESS)
		return ;
	else if (attack_monster_in_base(general) == SUCCESS)
		return ;
	else if (hit_closest_in_range(general, x, y, range) == SUCCESS)
		return ;
	else
		move_to(x, y);
}

void	check_patrouille(t_general *general, t_patrouille *patrouille)
{
	vector <t_hero> *hero_list;

	hero_list = _hero();
	if (general->atk.patrouille == 0
		&& (*hero_list)[1].coor.x == patrouille->xB
		&&  (*hero_list)[1].coor.y == patrouille->yB)
		general->atk.patrouille = 1;
	if (general->atk.patrouille == 1
		&& (*hero_list)[1].coor.x == patrouille->xA
		&&  (*hero_list)[1].coor.y == patrouille->yA)
		general->atk.patrouille = 0; 
}

int	patrouille_attaque(t_general *general, t_patrouille *patrouille)
{
	check_patrouille(general, patrouille);
	if (general->atk.patrouille == 0)
		move_to(patrouille->xB, patrouille->yB);
	else if (general->atk.patrouille == 1)
		move_to(patrouille->xA, patrouille->yA);
}

void	check_end_set_up(t_general *general, t_patrouille *patrouille)
{
	vector <t_hero> *hero_list;

	hero_list = _hero();
	if ((general->atk.set_up == false) 
		&& ((*hero_list)[1].coor.x == patrouille->xA)
		&& ((*hero_list)[1].coor.y == patrouille->yA))
		{
			general->atk.set_up = true;
		}
}

void	set_up(t_general *general, t_patrouille *patrouille)
{
	int xA = patrouille->xA;
	int yA = patrouille->yA;

	move_to(xA, yA);
	dprintf(2, "go set_up\n");
}

void	init_patrouille_coor(t_general *general, t_patrouille *patrouille)
{
	int xA = 11700;
	int yA = 8000;
	int xB = 14500;
	int yB = 4000;

	get_true_coor(general, &xA, &yA);
	get_true_coor(general, &xB, &yB);
	patrouille->xA = xA;
	patrouille->xB = xB;
	patrouille->yA = yA;
	patrouille->yB = yB;
}

int	cannon_1(t_general *general)
{
	vector <t_monstre>	*list_monstre;
	vector	<t_hero>	*list_hero;
	t_hero				hero;
	t_monstre			monstre;
	int					cible_x;
	int					cible_y;

	list_monstre = _monstre();
	list_hero = _hero();
	hero = (*list_hero)[general->hero_num];

	for (int i = 0; i < list_monstre->size(); i++)
	{
		monstre = (*list_monstre)[i];
		if (general->cannon_ready == true
			&& calculate_distance(monstre.coor.x, monstre.coor.y, hero.coor.x, hero.coor.y) <= 1280
			&& calculate_distance(monstre.coor.x, monstre.coor.y, hero.coor.x, hero.coor.y) > 920
			&& calculate_distance_to_enemy(general, monstre.coor.x, monstre.coor.y) < 6900
			&&(calculate_distance(monstre.coor.x, monstre.coor.y, 17630 - general->base_x, 9000 - general->base_y)
			> calculate_distance(hero.coor.x, hero.coor.y, 17630 - general->base_x, 9000 - general->base_y)))
			{
				cible_x = hero.coor.x + 17630 - general->base_x - monstre.coor.x;
				cible_y = hero.coor.y + 9000 - general->base_y - monstre.coor.y;
				if (general->hero_num == 1)
					printf("SPELL WIND %d %d\n", cible_x, cible_y);
				else
					printf("WAIT\n");
				return (SUCCESS);
			}
	}
	return (FAILURE);
}

int	cannon_2(t_general *general)
{
	vector <t_monstre>	*list_monstre;
	vector	<t_hero>	*list_hero;
	t_hero				hero;
	t_monstre			monstre;
	int					cible_x;
	int					cible_y;

	list_monstre = _monstre();
	list_hero = _hero();
	hero = (*list_hero)[general->hero_num];

	for (int i = 0; i < list_monstre->size(); i++)
	{
		monstre = (*list_monstre)[i];
		if (calculate_distance(monstre.coor.x, monstre.coor.y, hero.coor.x, hero.coor.y) <= 1280
			&& calculate_distance_to_enemy(general, monstre.coor.x, monstre.coor.y) < 4700
			&&(calculate_distance(monstre.coor.x, monstre.coor.y, 17630 - general->base_x, 9000 - general->base_y)
			<= calculate_distance(hero.coor.x, hero.coor.y, 17630 - general->base_x, 9000 - general->base_y)))
			{
				cible_x = hero.coor.x + 17630 - general->base_x - monstre.coor.x;
				cible_y = hero.coor.y + 9000 - general->base_y - monstre.coor.y;
				printf("SPELL WIND %d %d\n", cible_x, cible_y);
				general->cannon_ready = false;
				return (SUCCESS);
			}
	}
	return (FAILURE);
}

int	thales_x(t_general *general, int x, int range, int distance)
{
	int thales;

	thales = ((distance - range) * abs(17630 - general->base_x - x)) / distance;
	return(abs(17630 - general->base_x - thales));
}

int	thales_y(t_general *general, int y, int range, int distance)
{
	int thales;

	thales = ((distance - range) * abs(9000 - general->base_y - y)) / distance;
	return(abs(9000 - general->base_y - thales));
}

t_coor	find_hero_goal(t_general *general, int x_mon, int y_mon)
{
	t_coor goal;
	int range;

	range = 1000;

	goal.x = thales_x(general, x_mon, range, calculate_distance_to_enemy(general, x_mon, y_mon));
	goal.y = thales_y(general, y_mon, range, calculate_distance_to_enemy(general, x_mon, y_mon));
	return (goal);
}

int	ammo_available(t_general *general)
{
	vector	<t_monstre>	*list_monstre;
	vector	<t_hero>	*list_hero;
	t_monstre			monstre;
	t_hero				hero;
	t_coor				goal;

	list_monstre = _monstre();
	list_hero = _hero();
	hero = (*list_hero)[general->hero_num];
	for (int i = 0; i < list_monstre->size(); i++)
	{
		monstre = (*list_monstre)[i];
		if (calculate_distance_to_enemy(general, monstre.coor.x + monstre.coor.vx, monstre.coor.y + monstre.coor.vy) < 6900)
		{
			goal = find_hero_goal(general, monstre.coor.x + monstre.coor.vx, monstre.coor.y + monstre.coor.vy);
			//dprintf(2, "target +1 = %d\n goal.x = %d\n goal.y = %d\n\n", monstre.id, goal.x, goal.y);

			if (calculate_distance(goal.x, goal.y, hero.coor.x, hero.coor.y) <= 800)
			{
				move_to(goal.x, goal.y);
				general->cannon_ready = true;
				//dprintf(2, "distance < 800\n");
				//dprintf(2, "target +2 = %d\n goal.x = %d\n goal.y = %d\n\n", monstre.id, goal.x, goal.y);
				return (SUCCESS);
			}
		}
	}
	return (FAILURE);
}

int	ammo_available_future(t_general *general)
{
	vector	<t_monstre>	*list_monstre;
	vector	<t_hero>	*list_hero;
	t_monstre			monstre;
	t_hero				hero;
	t_coor				goal;

	list_monstre = _monstre();
	list_hero = _hero();
	hero = (*list_hero)[general->hero_num];
	for (int i = 0; i < list_monstre->size(); i++)
	{
		monstre = (*list_monstre)[i];
		if (calculate_distance_to_enemy(general, monstre.coor.x + 2 * monstre.coor.vx, monstre.coor.y + 2 * monstre.coor.vy) < 6900)
		{
			goal = find_hero_goal(general, monstre.coor.x + 2 * monstre.coor.vx, monstre.coor.y + 2 * monstre.coor.vy);
			//dprintf(2, "target +2 = %d\n goal.x = %d\n goal.y = %d\n\n", monstre.id, goal.x, goal.y);

			if (calculate_distance(goal.x, goal.y, hero.coor.x, hero.coor.y) <= 1600)
			{
				move_to(goal.x, goal.y);
				//dprintf(2, "distance < 1600\n");
				//dprintf(2, "target +2 = %d\n goal.x = %d\n goal.y = %d\n\n", monstre.id, goal.x, goal.y);
				return (SUCCESS);
			}
		}
	}
	return (FAILURE);
}

void	attaquant(t_general *general)
{
	t_patrouille	patrouille;

	init_patrouille_coor(general, &patrouille);
	check_end_set_up(general, &patrouille);
	if (general->atk.set_up == false)
		set_up(general, &patrouille);
	else if (cannon_2(general) == SUCCESS)
		return ;
	else if (cannon_1(general) == SUCCESS)
		return ;
	else if (ammo_available(general) == SUCCESS)
		return ;
	else if (ammo_available_future(general) == SUCCESS)
		return ;
	else
		patrouille_attaque(general, &patrouille);
}

void	hero_0(t_general *general)
{
	if (cast_shield() == SUCCESS)
		return ;
	else if (general->tours < EARLY_HERO_0)
		early_algo_hero_0(general);
	else
		defenseur(general);
}

void	hero_1(t_general *general)
{
	if (cast_shield() == SUCCESS)
		return ;
	else if (general->tours < EARLY_HERO_1)
		early_algo_hero_1(general);
	else
		attaquant(general);
}

void	hero_2(t_general *general)
{
	if (cast_shield() == SUCCESS)
		return ;
	else if (general->tours < EARLY_HERO_2)
		early_algo_hero_2(general);
	else
		attaquant(general);
}

void	action_algo(t_general *general)
{
	int	hero_num;
	
	hero_num = general->hero_num;
	if (hero_num == 0)
		hero_0(general);
	else if (hero_num == 1)
		hero_1(general);
	else if (hero_num == 2)
		hero_2(general);
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
