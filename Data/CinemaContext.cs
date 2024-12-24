using Microsoft.EntityFrameworkCore;

public class CinemaContext : DbContext
{
    public CinemaContext(DbContextOptions<CinemaContext> options) : base(options)
    {
    }

    public DbSet<User> Users { get; set; }
    public DbSet<Movie> Movies { get; set; }
    public DbSet<Screening> Screenings { get; set; }
    public DbSet<Hall> Halls { get; set; }
    public DbSet<Seat> Seats { get; set; }
    public DbSet<Reservation> Reservations { get; set; }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);

        // İlişkiler
        modelBuilder.Entity<Screening>()
            .HasOne(s => s.Movie)
            .WithMany(m => m.Screenings)
            .HasForeignKey(s => s.MovieId);

        modelBuilder.Entity<Screening>()
            .HasOne(s => s.Hall)
            .WithMany(h => h.Screenings)
            .HasForeignKey(s => s.HallId);

        modelBuilder.Entity<Seat>()
            .HasOne(s => s.Hall)
            .WithMany(h => h.Seats)
            .HasForeignKey(s => s.HallId);

        modelBuilder.Entity<Reservation>()
            .HasOne(r => r.User)
            .WithMany(u => u.Reservations)
            .HasForeignKey(r => r.UserId);

        modelBuilder.Entity<Reservation>()
            .HasOne(r => r.Screening)
            .WithMany(s => s.Reservations)
            .HasForeignKey(r => r.ScreeningId);

        modelBuilder.Entity<Reservation>()
            .HasOne(r => r.Seat)
            .WithMany(s => s.Reservations)
            .HasForeignKey(r => r.SeatId);
    }
} 