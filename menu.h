void addlinetomenu(edict_t *ent,char *line,int option);
void clearmenu(edict_t *ent);
void setmenuhandler(edict_t *ent,void (*optionselected)(edict_t *ent,int option));


void menuup(edict_t *ent);
void menudown(edict_t *ent);
void menuselect(edict_t *ent);

void initmenu(edict_t *ent);
void showmenu(edict_t *ent);
void closemenu(edict_t *ent);

void Menu_test(edict_t *ent);

typedef struct menumsg_s
{
	char	*msg;
	int	option;
} menumsg_t;


typedef struct menusystem_s
{
	qboolean 	menu_active;
	qboolean	displaymsg;
	void 		(*optionselected)(edict_t *ent,int option);
	menumsg_t	messages[12];
	int 		currentline;
	int		num_of_lines;
} menusystem_t;
