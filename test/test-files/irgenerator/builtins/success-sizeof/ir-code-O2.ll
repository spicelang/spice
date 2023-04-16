; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [20 x i8] c"Size of double: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [17 x i8] c"Size of int: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [19 x i8] c"Size of short: %d\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [18 x i8] c"Size of long: %d\0A\00", align 1
@printf.str.4 = private unnamed_addr constant [18 x i8] c"Size of byte: %d\0A\00", align 1
@printf.str.5 = private unnamed_addr constant [18 x i8] c"Size of char: %d\0A\00", align 1
@printf.str.6 = private unnamed_addr constant [20 x i8] c"Size of string: %d\0A\00", align 1
@printf.str.7 = private unnamed_addr constant [18 x i8] c"Size of bool: %d\0A\00", align 1
@printf.str.8 = private unnamed_addr constant [19 x i8] c"Size of int[]: %d\0A\00", align 1
@printf.str.9 = private unnamed_addr constant [18 x i8] c"Size of int*: %d\0A\00", align 1
@printf.str.10 = private unnamed_addr constant [29 x i8] c"Size of struct instance: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 64)
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i64 32)
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, i64 16)
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i64 64)
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4, i64 8)
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i64 8)
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.6, i64 64)
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7, i64 1)
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.8, i64 224)
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.9, i64 64)
  %11 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.10, i64 128)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
