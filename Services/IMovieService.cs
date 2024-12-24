public interface IMovieService
{
    Task<IEnumerable<MovieDTO>> GetAllMoviesAsync();
    Task<MovieDTO> GetMovieByIdAsync(int id);
    Task<MovieDTO> CreateMovieAsync(CreateMovieDTO movieDto);
    Task<MovieDTO> UpdateMovieAsync(int id, CreateMovieDTO movieDto);
    Task DeleteMovieAsync(int id);
} 