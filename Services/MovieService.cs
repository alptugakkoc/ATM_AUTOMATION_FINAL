using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using DTOs;
using Services;

public class MovieService : IMovieService
{
    private readonly CinemaContext _context;

    public MovieService(CinemaContext context)
    {
        _context = context;
    }

    public async Task<IEnumerable<MovieDTO>> GetAllMoviesAsync()
    {
        return await _context.Movies
            .Select(m => new MovieDTO
            {
                Id = m.Id,
                Title = m.Title,
                Description = m.Description,
                DurationMinutes = m.DurationMinutes,
                Genre = m.Genre
            })
            .ToListAsync();
    }

    public async Task<MovieDTO> GetMovieByIdAsync(int id)
    {
        var movie = await _context.Movies.FindAsync(id);
        if (movie == null) throw new KeyNotFoundException("Film bulunamadı");

        return new MovieDTO
        {
            Id = movie.Id,
            Title = movie.Title,
            Description = movie.Description,
            DurationMinutes = movie.DurationMinutes,
            Genre = movie.Genre
        };
    }

    public async Task<MovieDTO> CreateMovieAsync(CreateMovieDTO movieDto)
    {
        var movie = new Movie
        {
            Title = movieDto.Title,
            Description = movieDto.Description,
            DurationMinutes = movieDto.DurationMinutes,
            Genre = movieDto.Genre
        };

        _context.Movies.Add(movie);
        await _context.SaveChangesAsync();

        return new MovieDTO
        {
            Id = movie.Id,
            Title = movie.Title,
            Description = movie.Description,
            DurationMinutes = movie.DurationMinutes,
            Genre = movie.Genre
        };
    }

    
} 