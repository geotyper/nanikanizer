#pragma once
#include "expression.hpp"
#include "expression_node.hpp"
#include "math_util.hpp"

namespace nnk
{

	template <class T>
	class abs_expression_node : public expression_node<T>
	{
	public:

		typedef expression_node<T> node_type;
		typedef std::shared_ptr<node_type> node_pointer;

		explicit abs_expression_node(const node_pointer& base)
			: base_(base)
		{
		}

		virtual bool is_branch() override
		{
			return true;
		}

		virtual void forward() override
		{
			if (output().size() != base_->output().size())
				output().resize(base_->output().size());

			for (std::size_t i = 0; i < base_->output().size(); ++i)
				output()[i] = std::abs(base_->output()[i]);
		}

		virtual void backward() override
		{
			for (std::size_t i = 0; i < output_grad().size(); ++i)
				base_->output_grad()[i] += output_grad()[i] * sign(base_->output()[i]);
		}

		virtual void enumerate_children(const std::function<void(expression_node_base*)>& callback) override
		{
			callback(base_.get());
		}

	private:

		node_pointer base_;

	};

	template <class T>
	expression<T> abs(const expression<T>& base)
	{
		return expression<T>(std::make_shared<abs_expression_node<T>>(base.root()));
	}

}
