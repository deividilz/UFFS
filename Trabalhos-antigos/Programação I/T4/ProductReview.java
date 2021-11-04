class ProductReview extends Post implements Evaluable{
	private String brand;
	private int starts;

	public void setBrand(String brand){
		this.brand = brand;
	}

	public String getBrand(){
		return this.brand;
	}
	
	public void setStars(int starts){
		this.starts = starts;
	}

	public int getStarts(){
		return this.starts;
	}

	public void evaluate(int value){
		this.starts = value;
	}
	
	@Override
	public String show(){
		String txt = super.show()+"\nMarca do produto: "+this.brand+"\nEstrelas: "+this.starts+"\n";
		return txt;
	}
}