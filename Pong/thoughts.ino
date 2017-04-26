/*
 * Code Inspiration - https://www.youtube.com/watch?v=xIqeK2hzx1I
 * 
 * Just some thoughts:
 * 
 * its okay for the paddles to reach the corners because the ball's coords are precise, so its either hitting one or the other first.
 * if its hitting yours, you need to be there. 
 * 
 * 
 * how should i move that paddle?
 * i think they need to actually move by pixels. i dont want the case where a the edge pixel is lit up but the ball can seem like its
 * going through because it was going through the other side of the pixel.
 * i think i'll anchor it to one side and then always calculate its span when detecting collisions.
 * 
 * 
 * 
 * how should i control the ball with the paddle. 2 ideas:
 * 1) based on where on the paddle the ball hits - the more to the edge, the greater the angle the ball is returned
 * 
 * 2) based on how fast the paddle is moving.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */


