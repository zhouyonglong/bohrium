<!--(for op, opcode, optype, opcount, typesigs, layouts, broadcast in data)-->

// @!op!@ - @!opcode!@ - @!optype!@ - @!opcount!@ (A,A,K)
template <typename Tres, typename Tlhs>
inline
void @!op!@ (multi_array<Tres>& res, multi_array<Tlhs> &lhs, int64_t rhs)
{
    Runtime::instance().typecheck<@!opcode!@, Tres, Tlhs, int64_t>();

    // TODO:
    //  * Check axis
    //  * shape-check
    //  * move checks to sugar-layer
    if (rhs<0) {
        rhs = lhs.getRank()+rhs;
    }
    if (rhs >= (int64_t)lhs.getRank()) {
        throw std::runtime_error("Error: Axis out of bounds in reduction.\n");
    }
    Runtime::instance().enqueue(@!opcode!@, res, lhs, rhs);
}
<!--(end)-->
