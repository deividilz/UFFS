import java.util.*;

class Post{
	private String title;
	private Date date = new Date();
	private String content;
	private int likes;
	private int dislikes;

	public void setTitle(String title){
		this.title = title;
	}

	public String getTitle(){
		return this.title;
	}
	
	public void setContent(String content){
		this.content = content;
	}

	public String getContent(){
		return this.content;
	}
	
	public void setLikes(int likes){
		this.likes = likes;
	}

	public int getLikes(){
		return this.likes;
	}
	
	public void setDislikes(int dislikes){
		this.dislikes = dislikes;
	}

	public int getDislikes(){
		return this.dislikes;
	}

	public String show(){
		String txt = "Titulo: "+this.title+"\nConteudo: "+this.content+"\nData: "+this.date+"\nLikes: "+this.likes+
			"\nDeslikes: "+this.dislikes;
		return txt;
	}

	public void like(){
		this.likes++;
	}

	public void dislike(){
		this.dislikes++;
	}
}