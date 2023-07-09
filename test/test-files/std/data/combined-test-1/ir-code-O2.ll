; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Vector = type { ptr, i64, i64 }
%struct.Pair = type { i32, ptr }

@anon.string.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.1 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@printf.str.0 = private unnamed_addr constant [11 x i8] c"Hello %s!\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %pairVector = alloca %struct.Vector, align 8
  %1 = alloca %struct.Pair, align 8
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca %struct.Pair, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  %p1 = alloca %struct.Pair, align 8
  call void @"_ZN6VectorI18std/data/pair.PairIiPcEE4ctorEv"(ptr nonnull %pairVector) #2
  store i32 0, ptr %2, align 4
  store ptr @anon.string.0, ptr %3, align 8
  call void @_ZN4PairIiPcE4ctorERiRPc(ptr nonnull %1, ptr nonnull %2, ptr nonnull %3) #2
  call void @"_ZN6VectorI18std/data/pair.PairIiPcEE8pushBackERK18std/data/pair.PairIiPcE"(ptr nonnull %pairVector, ptr nonnull %1) #2
  store i32 1, ptr %5, align 4
  store ptr @anon.string.1, ptr %6, align 8
  call void @_ZN4PairIiPcE4ctorERiRPc(ptr nonnull %4, ptr nonnull %5, ptr nonnull %6) #2
  call void @"_ZN6VectorI18std/data/pair.PairIiPcEE8pushBackERK18std/data/pair.PairIiPcE"(ptr nonnull %pairVector, ptr nonnull %4) #2
  %7 = call ptr @"_ZN6VectorI18std/data/pair.PairIiPcEE3getEj"(ptr nonnull %pairVector, i32 1) #2
  %8 = load %struct.Pair, ptr %7, align 8
  %.fca.0.extract = extractvalue %struct.Pair %8, 0
  store i32 %.fca.0.extract, ptr %p1, align 8
  %.fca.1.extract = extractvalue %struct.Pair %8, 1
  %.fca.1.gep = getelementptr inbounds %struct.Pair, ptr %p1, i64 0, i32 1
  store ptr %.fca.1.extract, ptr %.fca.1.gep, align 8
  %9 = call ptr @_ZN4PairIiPcE9getSecondEv(ptr nonnull %p1) #2
  %10 = load ptr, ptr %9, align 8
  %11 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, ptr %10)
  call void @"_ZN6VectorI18std/data/pair.PairIiPcEE4dtorEv"(ptr nonnull %pairVector) #2
  ret i32 0
}

declare void @"_ZN6VectorI18std/data/pair.PairIiPcEE4ctorEv"(ptr) local_unnamed_addr

declare void @_ZN4PairIiPcE4ctorERiRPc(ptr, ptr, ptr) local_unnamed_addr

declare void @"_ZN6VectorI18std/data/pair.PairIiPcEE8pushBackERK18std/data/pair.PairIiPcE"(ptr, ptr) local_unnamed_addr

declare ptr @"_ZN6VectorI18std/data/pair.PairIiPcEE3getEj"(ptr, i32) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare ptr @_ZN4PairIiPcE9getSecondEv(ptr) local_unnamed_addr

declare void @"_ZN6VectorI18std/data/pair.PairIiPcEE4dtorEv"(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
