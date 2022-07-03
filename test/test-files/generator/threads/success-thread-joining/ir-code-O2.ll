; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str.1 = private unnamed_addr constant [18 x i8] c"Thread 1 finished\00", align 1
@str.2 = private unnamed_addr constant [18 x i8] c"Thread 2 finished\00", align 1
@str.3 = private unnamed_addr constant [18 x i8] c"Thread 3 finished\00", align 1

declare i32 @usleep(i32) local_unnamed_addr

define i32 @main() local_unnamed_addr {
entry:
  %0 = alloca {}, align 8
  %t1 = alloca i8*, align 8
  %t3 = alloca i8*, align 8
  %1 = alloca { i8**, i8** }, align 8
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @0, i64 0, i64 0), i8** %t3, align 8
  %2 = alloca i8*, align 8
  %3 = bitcast {}* %0 to i8*
  %4 = call i32 @pthread_create(i8** nonnull %2, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %3)
  %5 = load i8*, i8** %2, align 8
  store i8* %5, i8** %t1, align 8
  %6 = getelementptr inbounds { i8**, i8** }, { i8**, i8** }* %1, i64 0, i32 0
  store i8** %t1, i8*** %6, align 8
  %7 = getelementptr inbounds { i8**, i8** }, { i8**, i8** }* %1, i64 0, i32 1
  store i8** %t3, i8*** %7, align 8
  %8 = alloca i8*, align 8
  %9 = bitcast { i8**, i8** }* %1 to i8*
  %10 = call i32 @pthread_create(i8** nonnull %8, i8* null, i8* (i8*)* nonnull @_thread1, i8* nonnull %9)
  %11 = load i8*, i8** %8, align 8
  %12 = alloca i8*, align 8
  %13 = call i32 @pthread_create(i8** nonnull %12, i8* null, i8* (i8*)* nonnull @_thread2, i8* nonnull %3)
  %14 = load i8*, i8** %12, align 8
  store i8* %14, i8** %t3, align 8
  %15 = load i8*, i8** %t1, align 8
  %16 = call i32 @pthread_join(i8* %15, i8** null)
  %17 = call i32 @pthread_join(i8* %11, i8** null)
  %18 = load i8*, i8** %t3, align 8
  %19 = call i32 @pthread_join(i8* %18, i8** null)
  %puts = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([17 x i8], [17 x i8]* @str, i64 0, i64 0))
  ret i32 0
}

define internal noalias i8* @_thread0(i8* nocapture readnone %0) {
entry:
  %1 = tail call i32 @usleep(i32 300000)
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @str.1, i64 0, i64 0))
  ret i8* null
}

declare i32 @pthread_create(i8**, i8*, i8* (i8*)*, i8*) local_unnamed_addr

define internal noalias i8* @_thread1(i8* nocapture readonly %0) {
entry:
  %1 = bitcast i8* %0 to i8***
  %2 = load i8**, i8*** %1, align 8
  %3 = getelementptr inbounds i8, i8* %0, i64 8
  %4 = bitcast i8* %3 to i8***
  %5 = load i8**, i8*** %4, align 8
  %6 = load i8*, i8** %2, align 8
  %7 = tail call i32 @pthread_join(i8* %6, i8** null)
  %8 = load i8*, i8** %5, align 8
  %9 = tail call i32 @pthread_join(i8* %8, i8** null)
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @str.2, i64 0, i64 0))
  ret i8* null
}

declare i32 @pthread_join(i8*, i8**) local_unnamed_addr

define internal noalias i8* @_thread2(i8* nocapture readnone %0) {
entry:
  %1 = tail call i32 @usleep(i32 200000)
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @str.3, i64 0, i64 0))
  ret i8* null
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
