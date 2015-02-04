class abstract_astnode
{
	public:
		virtual void print () = 0;
		virtual std::string generate_code(const symbolTable&) = 0;
		virtual basic_types getType() = 0;
		virtual bool checkTypeofAST() = 0;
	protected:
		virtual void setType(basic_types) = 0;
	private:
		typeExp astnode_type;
};

int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}