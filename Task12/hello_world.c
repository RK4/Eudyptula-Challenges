#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/string.h>

#define DRIVER_AUTHOR "Mohsin Shan"
#define DRIVER_DESC "A simple driver that demonstrates usage of list"
#define NAME_LEN 20

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head list;
};

static LIST_HEAD(identity_list);

struct identity *identity_find(int id)
{
	struct identity *node;
	/* Loop the list */
	list_for_each_entry(node, &identity_list, list) {
		if (node->id == id) {
			return node;
			pr_debug("Node found with id %d", node->id);
		}
	}
	return NULL;
}

int identity_create(char *name, int id)
{
	int val = -EINVAL;

	struct identity *new_identity = NULL;
	/* If id already present in the list throw error */
	new_identity = identity_find(id);
	if (new_identity)
		return val;

	/* Check memory has been allocated properly */
	new_identity =  kmalloc(sizeof(struct identity), GFP_KERNEL);
	if (!new_identity)
		return val;

	/* Set parameters */
	new_identity->id = id;
	strncpy(new_identity->name, name, NAME_LEN);
	new_identity->name[NAME_LEN - 1] = '\0';
	new_identity->busy = false;

	/* Add to the list */
	list_add(&(new_identity->list), &identity_list);

	pr_debug("Node created with id: %d name: %s ",
			new_identity->id, new_identity->name);
	return 0;
}

void identity_destroy(int id)
{
	struct identity *node;
	/* Find node */
	node = identity_find(id);
	if (!node)
		return;

	/* Del node */
	pr_debug("Node deleted with id: %d name: %s ", node->id, node->name);
	list_del(&(node->list));
	kfree(node);
	return;
}

int init_module(void)
{
	struct identity *temp;

	if (identity_create("Alice", 1))
		pr_debug("Could not add into the list");
	if (identity_create("Bob", 2))
		pr_debug("Could not add into the list");
	if (identity_create("Dave", 3))
		pr_debug("Could not add into the list");
	if (identity_create("Gena", 10))
		pr_debug("Could not add into the list");

	temp = identity_find(3);
		pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return 0;
}

void cleanup_module(void)
{
	pr_debug("Goodby World.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
