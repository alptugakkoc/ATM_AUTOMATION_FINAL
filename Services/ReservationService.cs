using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using Models;
using DTOs;

public class ReservationService : IReservationService
{
    private readonly CinemaContext _context;

    public ReservationService(CinemaContext context)
    {
        _context = context;
    }

    public async Task<ReservationDTO> CreateReservationAsync(int userId, CreateReservationDTO reservationDto)
    {
        // Koltuk müsait mi kontrol et
        var existingReservation = await _context.Reservations
            .AnyAsync(r => r.ScreeningId == reservationDto.ScreeningId && 
                          r.SeatId == reservationDto.SeatId &&
                          r.Status != "Cancelled");

        if (existingReservation)
        {
            throw new InvalidOperationException("Bu koltuk zaten rezerve edilmiş.");
        }

        var reservation = new Reservation
        {
            UserId = userId,
            ScreeningId = reservationDto.ScreeningId,
            SeatId = reservationDto.SeatId,
            ReservationTime = DateTime.UtcNow,
            Status = "Active",
            Price = await CalculatePrice(reservationDto.ScreeningId)
        };

        _context.Reservations.Add(reservation);
        await _context.SaveChangesAsync();

        return new ReservationDTO
        {
            Id = reservation.Id,
            ScreeningId = reservation.ScreeningId,
            SeatId = reservation.SeatId,
            ReservationTime = reservation.ReservationTime,
            Price = reservation.Price,
            Status = reservation.Status
        };
    }

    public async Task<IEnumerable<ReservationDTO>> GetUserReservationsAsync(int userId)
    {
        return await _context.Reservations
            .Where(r => r.UserId == userId)
            .Select(r => new ReservationDTO
            {
                Id = r.Id,
                ScreeningId = r.ScreeningId,
                SeatId = r.SeatId,
                ReservationTime = r.ReservationTime,
                Price = r.Price,
                Status = r.Status
            })
            .ToListAsync();
    }

    public async Task<ReservationDTO> CancelReservationAsync(int userId, int reservationId)
    {
        var reservation = await _context.Reservations
            .FirstOrDefaultAsync(r => r.Id == reservationId && r.UserId == userId);

        if (reservation == null)
        {
            throw new KeyNotFoundException("Rezervasyon bulunamadı.");
        }

        if (reservation.Status == "Cancelled")
        {
            throw new InvalidOperationException("Bu rezervasyon zaten iptal edilmiş.");
        }

        reservation.Status = "Cancelled";
        await _context.SaveChangesAsync();

        return new ReservationDTO
        {
            Id = reservation.Id,
            ScreeningId = reservation.ScreeningId,
            SeatId = reservation.SeatId,
            ReservationTime = reservation.ReservationTime,
            Price = reservation.Price,
            Status = reservation.Status
        };
    }

    private async Task<decimal> CalculatePrice(int screeningId)
    {
        // Fiyat hesaplama 
        var screening = await _context.Screenings
            .Include(s => s.Movie)
            .FirstOrDefaultAsync(s => s.Id == screeningId);

        if (screening == null)
        {
            throw new KeyNotFoundException("Gösterim bulunamadı.");
        }

        // Temel fiyat
        decimal basePrice = 50.00m;

        // Hafta sonu ek ücreti
        if (screening.ScreeningTime.DayOfWeek == DayOfWeek.Saturday || 
            screening.ScreeningTime.DayOfWeek == DayOfWeek.Sunday)
        {
            basePrice += 10.00m;
        }

        return basePrice;
    }
} 