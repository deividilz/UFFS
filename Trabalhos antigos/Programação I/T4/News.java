class News extends Post{
	private String source;

	public void setSource(String source){
		this.source = source;
	}

	public String getSource(){
		return this.source;
	}

	 @Override
	public String show(){
		String txt = super.show()+"\nFonte da noticia: "+this.source+"\n";
		return txt;
	}
}