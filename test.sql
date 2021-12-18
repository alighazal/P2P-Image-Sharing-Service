/* 
insert into images_permission ( image_id, sent_from, sent_to, view_count ) values ( "1", "1", "2", "5" ); 
*/

/*
select 

images_permission.image_id,
images_permission.sent_from as sender_id,  
users.email as sender_email , 
images_permission.sent_to as recipiant_id, 
users2.email as recipiant_email , 
content as image_contnet 

from images_permission

join images 
on images_permission.image_id = images.image_id

join users 
on users.user_id = images_permission.sent_from

join users as users2
on users2.user_id = images_permission.sent_to

where images_permission.sent_to = "2" ;

*/


/*select  images_permission.image_id from images_permission where images_permission.sent_to =  ;*/


-- insert into images_permission ( image_id, sent_from, sent_to, view_count ) values ( "2", "1", "2", "5" ); 

select * from images where image_id = "1" limit 1;


