; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@str = private unnamed_addr constant [24 x i8] c"Starting one thread ...\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"Hello from original\00", align 1
@str.2 = private unnamed_addr constant [24 x i8] c"Hello from the thread 1\00", align 1
@str.3 = private unnamed_addr constant [24 x i8] c"Hello from the thread 2\00", align 1

declare i32 @usleep(i32) local_unnamed_addr

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca {}, align 8
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %2 = alloca ptr, align 8
  %3 = call i32 @pthread_create(ptr nonnull %2, ptr null, ptr nonnull @_thread0, ptr nonnull %1) #2
  %4 = alloca ptr, align 8
  %5 = call i32 @pthread_create(ptr nonnull %4, ptr null, ptr nonnull @_thread1, ptr nonnull %1) #2
  %6 = call i32 @usleep(i32 1000000) #2
  %puts1 = call i32 @puts(ptr nonnull dereferenceable(1) @str.1)
  ret i32 0
}

define private noalias ptr @_thread0(ptr nocapture readnone %0) {
  %2 = tail call i32 @usleep(i32 500000)
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr) local_unnamed_addr

define private noalias ptr @_thread1(ptr nocapture readnone %0) {
  %2 = tail call i32 @usleep(i32 200000)
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.3)
  ret ptr null
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
