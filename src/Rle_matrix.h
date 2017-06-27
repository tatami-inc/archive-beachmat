#ifndef BEACHMAT_RLE_MATRIX_H
#define BEACHMAT_RLE_MATRIX_H

template<typename T, class V>
class Rle_matrix : public any_matrix {
public:
    Rle_matrix(const Rcpp::RObject&);
    ~Rle_matrix();

    T get(size_t, size_t);

    template<class Iter>
    void get_row(size_t, Iter, size_t, size_t); 

    template<class Iter>
    void get_col(size_t, Iter, size_t, size_t); 

    matrix_type get_matrix_type () const;
private:
    std::deque<V> runvalues;
    std::vector<size_t> chunkdex, coldex;
    std::vector<std::deque<size_t> > cumrow;

    void initialize_solid_rle(const Rcpp::RObject&);
    void initialize_chunked_rle(const Rcpp::RObject&);
    size_t parse_rle(const Rcpp::IntegerVector&, size_t);

    size_t cache_row, cache_start, cache_end;
    std::vector<size_t> cache_indices;
    void update_indices(size_t r, size_t start, size_t end);
};

#endif
