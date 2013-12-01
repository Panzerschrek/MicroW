#include "monster.h"
#include "math.h"
#include "player.h"
#include "level.h"
#include "time.h"
#include "main_loop.h"

Monster::Monster( Player* p, Level* l, unsigned char monster_id, const float* pos )
{
    player= p;
    level= l;
    md= monster_descriptor_table + monster_id;

    this->monster_id= monster_id;

    target_pos[0]= this->pos[0]= pos[0];
    target_pos[1]= this->pos[1]= pos[1];
    target_pos[2]= this->pos[2]= pos[2];

    health= monster_descriptor_table[ monster_id ].max_health;
    last_attack_time= 0;
    last_target_select_time= 0;

    for( int i= 0; i< ANIMATION_KIND; i++ )
    {
        playable_animations[i]= false;
        playable_animations_time[i]= 0.0f;
    }

    in_remote_attack= false;
    in_melee_attack= false;
    alrady_caused_damage= false;
    monster_state= MONSTER_STATE_MOVE_TO_TARGET;
}



void Monster::SetPosition( const float* p )
{
    pos[0]= p[0];
    pos[1]= p[1];
    pos[2]= p[2];
}

bool Monster::MeleeAttack()
{
    float dt= float( MainLoop::CurrentTime() - last_attack_time ) / float( CLOCKS_PER_SEC );
    if( dt > md->melee_attack_delay && !in_remote_attack  )
    {
        last_attack_time= MainLoop::CurrentTime();
        alrady_caused_damage= false;
        in_melee_attack= true;
        playable_animations[ANIMATION_ATTACK0]= true;
        playable_animations_time[ANIMATION_ATTACK0] = 0.0f;
        playable_animations[ ANIMATION_WALK0 ]= false;

        return true;
    }
    else
        return false;
}
bool Monster::RemoteAttack()
{
    float dt= float( MainLoop::CurrentTime() - last_attack_time ) / float( CLOCKS_PER_SEC );
   if( dt > md->remote_attack_delay && !in_melee_attack  )
    {
        last_attack_time= MainLoop::CurrentTime();
        alrady_caused_damage= false;
        in_remote_attack= true;
        playable_animations[ANIMATION_ATTACK1]= true;
        playable_animations_time[ANIMATION_ATTACK1] = 0.0f;
        playable_animations[ ANIMATION_WALK0 ]= false;

        return true;
    }
   else
        return false;
}
    


void Monster::AttackTick()
{
    //MonsterDescriptor* md= &monster_descriptor_table[monster_id];
    float a_dt= float( MainLoop::CurrentTime() - last_attack_time ) / float( CLOCKS_PER_SEC );

    if( in_melee_attack )
    {
        TurnToPlayer();
        if ( a_dt > md->delay_before_melee_attack && a_dt <=  md->delay_before_melee_attack + md->delay_after_melee_attack )
        {
            if( ! alrady_caused_damage )
            {
                alrady_caused_damage= true;
                if(  Distance( pos, player->Position() ) < 2.0f )
                {
                    player->Damage( md->melee_damage );
                    level->GetSoundSystem()->PlaySound( pos, 1.0f, SOUND_SHOVEL_HIT );
                }

                SelectTarget();
                monster_state = MONSTER_STATE_MOVE_TO_TARGET;
            }
        }
        else if( a_dt > md->delay_before_melee_attack + md->delay_after_melee_attack )
        {
           in_melee_attack= false;
           playable_animations[ANIMATION_ATTACK0]= false;
           
         //  SelectTarget();
         //  monster_state = MONSTER_STATE_MOVE_TO_TARGET;
         //  printf( "end melee attack\n" );
        }
    }
    else if( in_remote_attack )
    {
        TurnToPlayer();
       // printf( "remote attack\n" );
        if ( a_dt > md->delay_before_remote_attack && a_dt <=  md->delay_before_remote_attack + md->delay_after_remote_attack )
        {
            if( ! alrady_caused_damage )
            {
                alrady_caused_damage= true;
                if(  Distance( pos, player->Position() ) < 9000.0f )
                {
                    Shot();
                    //player->Damage( md->remote_damage );
                    //level->GetSoundSystem()->PlaySound( pos, 1.0f, SOUND_BLASTER_SHOT );
                }

                SelectTarget();
                monster_state = MONSTER_STATE_MOVE_TO_TARGET;
            }
        }
        else if( a_dt > md->delay_before_remote_attack + md->delay_after_remote_attack )
        {
           in_remote_attack= false;
           playable_animations[ANIMATION_ATTACK1]= false;
          // SelectTarget();
          // monster_state = MONSTER_STATE_MOVE_TO_TARGET;
          // printf( "end remote attack\n" );
        }
    }
}

void Monster::SelectTarget()
{
    //if( Distance(target_pos, pos ) < 0.5f )
   // {
        float l= Distance( player->Position(), pos );
        float eps_vec[]= { randf(-1.0f,1.0f), randf(-1.0f,1.0f), 0.0f };
        Vec3Mul( eps_vec, 0.3f*l );

        float vec_2_target[]= { player->Position()[0], player->Position()[1], player->Position()[2] };
        Vec3Sub( vec_2_target, pos );
        Vec3Normalize( vec_2_target );
        Vec3Mul( vec_2_target, randf( 10.0f, 1.0f ) );//select distance to target

        target_pos[0]= pos[0];
        target_pos[1]= pos[1];
        target_pos[2]= pos[2];
        Vec3Add( target_pos, vec_2_target );

		if( monster_id== MONSTER_CACODEMON )
			if( Distance( player->Position(), pos ) > 4.0f )
				target_pos[2]= level->GetLandscape()->Height( target_pos[0], target_pos[1] ) + 3.5f;

   // }
     //   printf( "select target\n" );
}

void Monster::TurnToPlayer()
{
    float vec[3]= { cos(angle_xy+MW_PI2), sin( angle_xy+MW_PI2 ), 0.0f };

    float vec_2_player[3]= { player->Position()[0], player->Position()[1], player->Position()[2] };
    Vec3Sub( vec_2_player, pos );
    Vec3Normalize( vec_2_player );

    Vec3Mul( vec_2_player, 0.08f );
    Vec3Mul( vec, 0.92f );
    Vec3Add( vec_2_player, vec );
    Vec3Normalize( vec_2_player );

    angle_xy= Vec3XYAngle( vec_2_player ) - MW_PI2;
}


void Monster::Move()
{
    if( Distance( pos, target_pos ) < 0.5f )
    {
        if( (rand()&1) )
        {
            if( RemoteAttack() )
                monster_state= MONSTER_STATE_REMOTE_ATTACK;
            else
                 SelectTarget();
        }
        else
            SelectTarget();
    }
    else
    {
        float vec_to_target[]= { target_pos[0], target_pos[1], target_pos[2] };
        Vec3Sub( vec_to_target, pos );
        
        Vec3Normalize( vec_to_target );

        Vec3Mul( vec_to_target, MainLoop::FrameDT() * md->speed );
        Vec3Add( pos, vec_to_target );

        angle_xy= Vec3XYAngle( vec_to_target ) - MW_PI2;
        playable_animations[ ANIMATION_WALK0 ]= true;
      //  printf( "move\n" );
    }
}

void Monster::Tick()
{
    AttackTick();

    if( player->Health() > 0 )
    {
        if( Distance( player->Position(), pos ) <= 2.0f )
        {
            if( MeleeAttack() )
                monster_state= MONSTER_STATE_MELEE_ATTACK;
        }
        else if( monster_state == MONSTER_STATE_MOVE_TO_TARGET )
            Move();

    }//if player is death, do nothing

    for( int i= 0; i< MAX_MODEL_ANIMATIONS; i++ )
         playable_animations_time[i]+= MainLoop::FrameDT();

    if( ! MONSTER_CAN_FLY( md->flags ) )
        pos[2]-= MainLoop::FrameDT() * 4.0f;
    
    float h= level->GetLandscape()->Height( pos[0], pos[1] );
    float dh= 0.0f;
    if( MONSTER_IS_SPHERICAL( md->flags) )
        dh= md->radius;
    if( pos[2] - dh < h )
        pos[2]= h + dh;
		
}


void Monster::Shot()
{
    float m_pos[]= { pos[0], pos[1], pos[2] };
    if( MONSTER_IS_CYLINDRICAL(md->flags) )
        m_pos[2]+= md->heigth;

    float vec_2_player[]= { player->Position()[0], player->Position()[1], player->Position()[2] + 0.5f * PLAYER_HEIGHT };
    Vec3Sub( vec_2_player, m_pos );

    if( monster_id == MONSTER_SNOWMAN )
    {
        float delta[]= { randf(-1.0f, 1.0f ), randf(-1.0f, 1.0f ), randf(-1.0f, 1.0f ) };
        Vec3Mul( delta, Vec3Len( vec_2_player ) * 0.02f );
        Vec3Add( vec_2_player, delta );

        float l= Vec3Len( vec_2_player );
        float dx2= vec_2_player[0] * vec_2_player[0] + vec_2_player[1] * vec_2_player[1];
        float c1, c2;
        c1= ( GRAVITY * dx2 / (25.0f*25.0f) - vec_2_player[2] ) / l;
        c2= vec_2_player[2] / l;
        if( fabs(c1) >= 1.0f )
            return;
        float a= 0.5f * ( acos(c1) - acos(c2) );
        vec_2_player[2]= sqrt(dx2) * tan(a);
    }
        Vec3Normalize( vec_2_player );


    level->MonsterShot( m_pos, vec_2_player, monster_id );

}
bool Monster::IsCollision( const float* pos, const float* vec, float* collision_pos )
{
    if( MONSTER_IS_SPHERICAL(md->flags) )
        return FindCollisionWithSphere( pos, vec, this->pos, md->radius, collision_pos );
    else
        return FindCollisionWithCylinder( pos, vec, this->pos, md->radius, md->heigth, collision_pos );
}