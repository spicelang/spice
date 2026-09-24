; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Obj = type { i32 }
%struct.Pair = type { %struct.String, %struct.String }
%struct.String = type { ptr, i64, i64 }

@printf.str.0 = private unnamed_addr constant [9 x i8] c"Ctor %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [14 x i8] c"Copy ctor %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [9 x i8] c"Dtor %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [40 x i8] c"Struct lvalues bound to non-const ref:\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [11 x i8] c"a=%d b=%d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [50 x i8] c"Struct lvalues bound to const ref and discarded:\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [8 x i8] c"ref=%d\0A\00", align 4
@printf.str.7 = private unnamed_addr constant [51 x i8] c"Struct lvalues passed to non-const ref parameter:\0A\00", align 4
@printf.str.8 = private unnamed_addr constant [11 x i8] c"a=%d b=%d\0A\00", align 4
@printf.str.9 = private unnamed_addr constant [33 x i8] c"Struct lvalues copied by value:\0A\00", align 4
@printf.str.10 = private unnamed_addr constant [14 x i8] c"b=%d copy=%d\0A\00", align 4
@printf.str.11 = private unnamed_addr constant [16 x i8] c"Struct fields:\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [2 x i8] c"a\00", align 4
@anon.string.1 = private unnamed_addr constant [2 x i8] c"b\00", align 4
@anon.string.2 = private unnamed_addr constant [2 x i8] c"x\00", align 4
@anon.string.3 = private unnamed_addr constant [2 x i8] c"y\00", align 4
@printf.str.12 = private unnamed_addr constant [7 x i8] c"%s %s\0A\00", align 4
@printf.str.13 = private unnamed_addr constant [13 x i8] c"Primitives:\0A\00", align 4
@printf.str.14 = private unnamed_addr constant [11 x i8] c"x=%d y=%d\0A\00", align 4
@printf.str.15 = private unnamed_addr constant [17 x i8] c"Nested ternary:\0A\00", align 4
@printf.str.16 = private unnamed_addr constant [16 x i8] c"x=%d y=%d z=%d\0A\00", align 4
@printf.str.17 = private unnamed_addr constant [16 x i8] c"a=%d b=%d c=%d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %0, i32 noundef %1) #0 {
  %this = alloca ptr, align 8
  %id = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store i32 %1, ptr %id, align 4
  %3 = load ptr, ptr %this, align 8
  store i32 0, ptr %3, align 4
  %4 = load ptr, ptr %this, align 8
  %id.addr = getelementptr inbounds %struct.Obj, ptr %4, i64 0, i32 0
  %5 = load i32, ptr %id, align 4
  store i32 %5, ptr %id.addr, align 4
  %6 = load ptr, ptr %this, align 8
  %id.addr1 = getelementptr inbounds %struct.Obj, ptr %6, i64 0, i32 0
  %7 = load i32, ptr %id.addr1, align 4
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %7)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN3Obj4ctorERK3Obj(ptr noundef nonnull align 4 dereferenceable(4) %0, ptr noundef %1) #0 {
  %this = alloca ptr, align 8
  %other = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %other, align 8
  %3 = load ptr, ptr %this, align 8
  store i32 0, ptr %3, align 4
  %4 = load ptr, ptr %this, align 8
  %id.addr = getelementptr inbounds %struct.Obj, ptr %4, i64 0, i32 0
  %5 = load ptr, ptr %other, align 8
  %id.addr1 = getelementptr inbounds %struct.Obj, ptr %5, i64 0, i32 0
  %6 = load i32, ptr %id.addr1, align 4
  store i32 %6, ptr %id.addr, align 4
  %7 = load ptr, ptr %this, align 8
  %id.addr2 = getelementptr inbounds %struct.Obj, ptr %7, i64 0, i32 0
  %8 = load i32, ptr %id.addr2, align 4
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %8)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %id.addr = getelementptr inbounds %struct.Obj, ptr %2, i64 0, i32 0
  %3 = load i32, ptr %id.addr, align 4
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %3)
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN4Pair4dtorEv(ptr noundef nonnull align 8 dereferenceable(48) %0) #2 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds nuw %struct.Pair, ptr %2, i32 0, i32 1
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %3)
  %4 = load ptr, ptr %this, align 8
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %4)
  ret void
}

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Pair6appendEbRKPKc(ptr noundef nonnull align 8 dereferenceable(48) %0, i1 noundef zeroext %1, ptr noundef %2) #0 {
  %this = alloca ptr, align 8
  %cond = alloca i1, align 1
  %suffix = alloca ptr, align 8
  %target = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store i1 %1, ptr %cond, align 1
  store ptr %2, ptr %suffix, align 8
  %4 = load i1, ptr %cond, align 1
  br i1 %4, label %cond.true.L24C22, label %cond.false.L24C22

cond.true.L24C22:                                 ; preds = %3
  %5 = load ptr, ptr %this, align 8
  %first.addr = getelementptr inbounds %struct.Pair, ptr %5, i64 0, i32 0
  br label %cond.exit.L24C22

cond.false.L24C22:                                ; preds = %3
  %6 = load ptr, ptr %this, align 8
  %second.addr = getelementptr inbounds %struct.Pair, ptr %6, i64 0, i32 1
  br label %cond.exit.L24C22

cond.exit.L24C22:                                 ; preds = %cond.false.L24C22, %cond.true.L24C22
  %cond.result = phi ptr [ %first.addr, %cond.true.L24C22 ], [ %second.addr, %cond.false.L24C22 ]
  store ptr %cond.result, ptr %target, align 8
  %7 = load ptr, ptr %target, align 8
  %8 = load ptr, ptr %suffix, align 8
  call void @_Z12op.plusequalIPKcEvR6StringRKPKc(ptr %7, ptr %8)
  ret void
}

declare void @_Z12op.plusequalIPKcEvR6StringRKPKc(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z5setIdR3Obji(ptr noundef %0, i32 noundef %1) #0 {
  %obj = alloca ptr, align 8
  %id = alloca i32, align 4
  store ptr %0, ptr %obj, align 8
  store i32 %1, ptr %id, align 4
  %3 = load ptr, ptr %obj, align 8
  %id.addr = getelementptr inbounds %struct.Obj, ptr %3, i64 0, i32 0
  %4 = load i32, ptr %id, align 4
  store i32 %4, ptr %id.addr, align 4
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #3 {
  %falseCond = alloca i1, align 1
  %trueCond = alloca i1, align 1
  %a = alloca %struct.Obj, align 8
  %b = alloca %struct.Obj, align 8
  %ref = alloca ptr, align 8
  %a3 = alloca %struct.Obj, align 8
  %b4 = alloca %struct.Obj, align 8
  %ref6 = alloca ptr, align 8
  %a9 = alloca %struct.Obj, align 8
  %b10 = alloca %struct.Obj, align 8
  %a14 = alloca %struct.Obj, align 8
  %b15 = alloca %struct.Obj, align 8
  %copy = alloca %struct.Obj, align 8
  %1 = alloca %struct.String, align 8
  %2 = alloca %struct.String, align 8
  %pair = alloca %struct.Pair, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %ref21 = alloca ptr, align 8
  %x22 = alloca i32, align 4
  %y23 = alloca i32, align 4
  %z = alloca i32, align 4
  %ref26 = alloca ptr, align 8
  %a27 = alloca %struct.Obj, align 8
  %b28 = alloca %struct.Obj, align 8
  %c = alloca %struct.Obj, align 8
  %objRef = alloca ptr, align 8
  store i1 false, ptr %falseCond, align 1
  store i1 true, ptr %trueCond, align 1
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %a, i32 noundef 1)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %b, i32 noundef 2)
  %6 = load i1, ptr %falseCond, align 1
  br i1 %6, label %cond.true.L40C20, label %cond.false.L40C20

cond.true.L40C20:                                 ; preds = %0
  br label %cond.exit.L40C20

cond.false.L40C20:                                ; preds = %0
  br label %cond.exit.L40C20

cond.exit.L40C20:                                 ; preds = %cond.false.L40C20, %cond.true.L40C20
  %cond.result = phi ptr [ %a, %cond.true.L40C20 ], [ %b, %cond.false.L40C20 ]
  store ptr %cond.result, ptr %ref, align 8
  %7 = load ptr, ptr %ref, align 8
  %id.addr = getelementptr inbounds %struct.Obj, ptr %7, i64 0, i32 0
  store i32 20, ptr %id.addr, align 4
  %id.addr1 = getelementptr inbounds %struct.Obj, ptr %a, i64 0, i32 0
  %8 = load i32, ptr %id.addr1, align 4
  %id.addr2 = getelementptr inbounds %struct.Obj, ptr %b, i64 0, i32 0
  %9 = load i32, ptr %id.addr2, align 4
  %10 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %8, i32 noundef %9)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %b)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %a)
  %11 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %a3, i32 noundef 3)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %b4, i32 noundef 4)
  %12 = load i1, ptr %trueCond, align 1
  br i1 %12, label %cond.true.L49C26, label %cond.false.L49C26

cond.true.L49C26:                                 ; preds = %cond.exit.L40C20
  br label %cond.exit.L49C26

cond.false.L49C26:                                ; preds = %cond.exit.L40C20
  br label %cond.exit.L49C26

cond.exit.L49C26:                                 ; preds = %cond.false.L49C26, %cond.true.L49C26
  %cond.result5 = phi ptr [ %a3, %cond.true.L49C26 ], [ %b4, %cond.false.L49C26 ]
  store ptr %cond.result5, ptr %ref6, align 8
  %13 = load i1, ptr %trueCond, align 1
  br i1 %13, label %cond.true.L50C9, label %cond.false.L50C9

cond.true.L50C9:                                  ; preds = %cond.exit.L49C26
  br label %cond.exit.L50C9

cond.false.L50C9:                                 ; preds = %cond.exit.L49C26
  br label %cond.exit.L50C9

cond.exit.L50C9:                                  ; preds = %cond.false.L50C9, %cond.true.L50C9
  %cond.result7 = phi ptr [ %a3, %cond.true.L50C9 ], [ %b4, %cond.false.L50C9 ]
  %14 = load ptr, ptr %ref6, align 8
  %id.addr8 = getelementptr inbounds %struct.Obj, ptr %14, i64 0, i32 0
  %15 = load i32, ptr %id.addr8, align 4
  %16 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i32 noundef %15)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %b4)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %a3)
  %17 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.7)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %a9, i32 noundef 5)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %b10, i32 noundef 6)
  %18 = load i1, ptr %trueCond, align 1
  br i1 %18, label %cond.true.L58C15, label %cond.false.L58C15

cond.true.L58C15:                                 ; preds = %cond.exit.L50C9
  br label %cond.exit.L58C15

cond.false.L58C15:                                ; preds = %cond.exit.L50C9
  br label %cond.exit.L58C15

cond.exit.L58C15:                                 ; preds = %cond.false.L58C15, %cond.true.L58C15
  %cond.result11 = phi ptr [ %a9, %cond.true.L58C15 ], [ %b10, %cond.false.L58C15 ]
  call void @_Z5setIdR3Obji(ptr noundef %cond.result11, i32 noundef 50)
  %id.addr12 = getelementptr inbounds %struct.Obj, ptr %a9, i64 0, i32 0
  %19 = load i32, ptr %id.addr12, align 4
  %id.addr13 = getelementptr inbounds %struct.Obj, ptr %b10, i64 0, i32 0
  %20 = load i32, ptr %id.addr13, align 4
  %21 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.8, i32 noundef %19, i32 noundef %20)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %b10)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %a9)
  %22 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.9)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %a14, i32 noundef 7)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %b15, i32 noundef 8)
  %23 = load i1, ptr %falseCond, align 1
  br i1 %23, label %cond.true.L66C20, label %cond.false.L66C20

cond.true.L66C20:                                 ; preds = %cond.exit.L58C15
  br label %cond.exit.L66C20

cond.false.L66C20:                                ; preds = %cond.exit.L58C15
  br label %cond.exit.L66C20

cond.exit.L66C20:                                 ; preds = %cond.false.L66C20, %cond.true.L66C20
  %cond.result16 = phi ptr [ %a14, %cond.true.L66C20 ], [ %b15, %cond.false.L66C20 ]
  call void @_ZN3Obj4ctorERK3Obj(ptr noundef nonnull align 4 dereferenceable(4) %copy, ptr %cond.result16)
  %id.addr17 = getelementptr inbounds %struct.Obj, ptr %copy, i64 0, i32 0
  store i32 80, ptr %id.addr17, align 4
  %id.addr18 = getelementptr inbounds %struct.Obj, ptr %b15, i64 0, i32 0
  %24 = load i32, ptr %id.addr18, align 4
  %id.addr19 = getelementptr inbounds %struct.Obj, ptr %copy, i64 0, i32 0
  %25 = load i32, ptr %id.addr19, align 4
  %26 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.10, i32 noundef %24, i32 noundef %25)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %copy)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %b15)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %a14)
  %27 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.11)
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr noundef @anon.string.0)
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr noundef @anon.string.1)
  %28 = load %struct.String, ptr %1, align 8
  store %struct.String %28, ptr %pair, align 8
  %29 = load %struct.String, ptr %2, align 8
  %30 = getelementptr inbounds nuw %struct.Pair, ptr %pair, i32 0, i32 1
  store %struct.String %29, ptr %30, align 8
  store ptr @anon.string.2, ptr %3, align 8
  call void @_ZN4Pair6appendEbRKPKc(ptr noundef nonnull align 8 dereferenceable(48) %pair, i1 noundef zeroext true, ptr noundef %3)
  store ptr @anon.string.3, ptr %4, align 8
  call void @_ZN4Pair6appendEbRKPKc(ptr noundef nonnull align 8 dereferenceable(48) %pair, i1 noundef zeroext false, ptr noundef %4)
  %31 = call noundef ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %pair)
  %32 = getelementptr inbounds nuw %struct.Pair, ptr %pair, i32 0, i32 1
  %33 = call noundef ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %32)
  %34 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.12, ptr noundef %31, ptr noundef %33)
  call void @_ZN4Pair4dtorEv(ptr noundef nonnull align 8 dereferenceable(48) %pair)
  %35 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.13)
  store i32 1, ptr %x, align 4
  store i32 2, ptr %y, align 4
  %36 = load i1, ptr %falseCond, align 1
  br i1 %36, label %cond.true.L83C20, label %cond.false.L83C20

cond.true.L83C20:                                 ; preds = %cond.exit.L66C20
  br label %cond.exit.L83C20

cond.false.L83C20:                                ; preds = %cond.exit.L66C20
  br label %cond.exit.L83C20

cond.exit.L83C20:                                 ; preds = %cond.false.L83C20, %cond.true.L83C20
  %cond.result20 = phi ptr [ %x, %cond.true.L83C20 ], [ %y, %cond.false.L83C20 ]
  store ptr %cond.result20, ptr %ref21, align 8
  %37 = load ptr, ptr %ref21, align 8
  store i32 42, ptr %37, align 4
  %38 = load i32, ptr %x, align 4
  %39 = load i32, ptr %y, align 4
  %40 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.14, i32 noundef %38, i32 noundef %39)
  %41 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.15)
  store i32 1, ptr %x22, align 4
  store i32 2, ptr %y23, align 4
  store i32 3, ptr %z, align 4
  %42 = load i1, ptr %falseCond, align 1
  br i1 %42, label %cond.true.L93C20, label %cond.false.L93C20

cond.true.L93C20:                                 ; preds = %cond.exit.L83C20
  br label %cond.exit.L93C20

cond.false.L93C20:                                ; preds = %cond.exit.L83C20
  %43 = load i1, ptr %falseCond, align 1
  br i1 %43, label %cond.true.L93C37, label %cond.false.L93C37

cond.true.L93C37:                                 ; preds = %cond.false.L93C20
  br label %cond.exit.L93C37

cond.false.L93C37:                                ; preds = %cond.false.L93C20
  br label %cond.exit.L93C37

cond.exit.L93C37:                                 ; preds = %cond.false.L93C37, %cond.true.L93C37
  %cond.result24 = phi ptr [ %y23, %cond.true.L93C37 ], [ %z, %cond.false.L93C37 ]
  br label %cond.exit.L93C20

cond.exit.L93C20:                                 ; preds = %cond.exit.L93C37, %cond.true.L93C20
  %cond.result25 = phi ptr [ %x22, %cond.true.L93C20 ], [ %cond.result24, %cond.exit.L93C37 ]
  store ptr %cond.result25, ptr %ref26, align 8
  %44 = load ptr, ptr %ref26, align 8
  store i32 42, ptr %44, align 4
  %45 = load i32, ptr %x22, align 4
  %46 = load i32, ptr %y23, align 4
  %47 = load i32, ptr %z, align 4
  %48 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.16, i32 noundef %45, i32 noundef %46, i32 noundef %47)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %a27, i32 noundef 9)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %b28, i32 noundef 10)
  call void @_ZN3Obj4ctorEi(ptr noundef nonnull align 4 dereferenceable(4) %c, i32 noundef 11)
  %49 = load i1, ptr %trueCond, align 1
  br i1 %49, label %cond.true.L99C23, label %cond.false.L99C23

cond.true.L99C23:                                 ; preds = %cond.exit.L93C20
  %50 = load i1, ptr %falseCond, align 1
  br i1 %50, label %cond.true.L99C35, label %cond.false.L99C35

cond.true.L99C35:                                 ; preds = %cond.true.L99C23
  br label %cond.exit.L99C35

cond.false.L99C35:                                ; preds = %cond.true.L99C23
  br label %cond.exit.L99C35

cond.exit.L99C35:                                 ; preds = %cond.false.L99C35, %cond.true.L99C35
  %cond.result29 = phi ptr [ %a27, %cond.true.L99C35 ], [ %b28, %cond.false.L99C35 ]
  br label %cond.exit.L99C23

cond.false.L99C23:                                ; preds = %cond.exit.L93C20
  br label %cond.exit.L99C23

cond.exit.L99C23:                                 ; preds = %cond.false.L99C23, %cond.exit.L99C35
  %cond.result30 = phi ptr [ %cond.result29, %cond.exit.L99C35 ], [ %c, %cond.false.L99C23 ]
  store ptr %cond.result30, ptr %objRef, align 8
  %51 = load ptr, ptr %objRef, align 8
  %id.addr31 = getelementptr inbounds %struct.Obj, ptr %51, i64 0, i32 0
  store i32 100, ptr %id.addr31, align 4
  %id.addr32 = getelementptr inbounds %struct.Obj, ptr %a27, i64 0, i32 0
  %52 = load i32, ptr %id.addr32, align 4
  %id.addr33 = getelementptr inbounds %struct.Obj, ptr %b28, i64 0, i32 0
  %53 = load i32, ptr %id.addr33, align 4
  %id.addr34 = getelementptr inbounds %struct.Obj, ptr %c, i64 0, i32 0
  %54 = load i32, ptr %id.addr34, align 4
  %55 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.17, i32 noundef %52, i32 noundef %53, i32 noundef %54)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %c)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %b28)
  call void @_ZN3Obj4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %a27)
  ret i32 0
}

declare void @_ZN6String4ctorEPKc(ptr, ptr)

declare ptr @_ZN6String6getRawEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { mustprogress noinline nounwind optnone uwtable }
attributes #3 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
