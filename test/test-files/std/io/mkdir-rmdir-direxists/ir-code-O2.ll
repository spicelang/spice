; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [28 x i8] c"Existing before create: %d\0A\00", align 1
@MODE_ALL_RWX = external local_unnamed_addr global i32
@printf.str.1 = private unnamed_addr constant [23 x i8] c"mkDir return code: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [27 x i8] c"Existing after create: %d\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [23 x i8] c"rmDir return code: %d\0A\00", align 1
@printf.str.4 = private unnamed_addr constant [27 x i8] c"Existing after delete: %d\0A\00", align 1
@anon.string.4 = private unnamed_addr constant [7 x i8] c"./test\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call i1 @_f__void__bool__dirExists__string(ptr nonnull @anon.string.4) #2
  %2 = zext i1 %1 to i32
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 %2)
  %4 = load i32, ptr @MODE_ALL_RWX, align 4
  %5 = tail call i32 @_f__void__int__mkDir__string_int(ptr nonnull @anon.string.4, i32 %4) #2
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 %5)
  %7 = tail call i1 @_f__void__bool__dirExists__string(ptr nonnull @anon.string.4) #2
  %8 = zext i1 %7 to i32
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.2, i32 %8)
  %10 = tail call i32 @_f__void__int__rmDir__string(ptr nonnull @anon.string.4) #2
  %11 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.3, i32 %10)
  %12 = tail call i1 @_f__void__bool__dirExists__string(ptr nonnull @anon.string.4) #2
  %13 = zext i1 %12 to i32
  %14 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.4, i32 %13)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i1 @_f__void__bool__dirExists__string(ptr) local_unnamed_addr

declare i32 @_f__void__int__mkDir__string_int(ptr, i32) local_unnamed_addr

declare i32 @_f__void__int__rmDir__string(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
