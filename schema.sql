CREATE TABLE IF NOT EXISTS users ( 
	user_id INTEGER PRIMARY KEY AUTOINCREMENT ,
	first_name TEXT NOT NULL,
	last_name TEXT NOT NULL,
	email TEXT NOT NULL UNIQUE,
	password TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS images ( 
	
	image_id INTEGER PRIMARY KEY AUTOINCREMENT,
	content TEXT,
	user_id INTEGER NOT NULL,

	FOREIGN KEY(user_id) REFERENCES users(user_id)
);

CREATE TABLE IF NOT EXISTS images_permission ( 
	
	image_id INTEGER NOT NULL  ,
	sent_from INTEGER NOT NULL,
	sent_to INTEGER NOT NULL,
	view_count INTEGER NOT NULL,

	PRIMARY KEY( image_id, sent_from, sent_to  ),
	FOREIGN KEY(sent_from) REFERENCES users(user_id),
	FOREIGN KEY(sent_to) REFERENCES users(user_id),
	FOREIGN KEY(image_id) REFERENCES images(image_id)
);





